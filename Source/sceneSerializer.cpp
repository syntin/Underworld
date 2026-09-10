//DB
#include "sceneSerializer.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "scene.h"
#include "entity.h"
#include "hierarchy.h"
#include "componentManager.h"
#include "entityManager.h"

// --- Save helpers ---

static void SaveTransform(std::ofstream& file, const Transform& t)
{
    file << "Component Transform\n";
    file << "    Position " << t.position.x << " " << t.position.y << " " << t.position.z << "\n";
    file << "    Rotation " << t.rotation.x << " " << t.rotation.y << " " << t.rotation.z << "\n";
    file << "    Scale " << t.scale.x << " " << t.scale.y << " " << t.scale.z << "\n";
}

static void SaveVelocity(std::ofstream& file, const Velocity& v)
{
    file << "Component Velocity\n";
    file << "    Linear " << v.linear.x << " " << v.linear.y << " " << v.linear.z << "\n";
    file << "    Angular " << v.angular.x << " " << v.angular.y << " " << v.angular.z << "\n";
}

static void SaveHealth(std::ofstream& file, const Health& h)
{
    file << "Component Health\n";
    file << "    Current " << h.current << "\n";
    file << "    Max " << h.max << "\n";
}

static void SaveLight(std::ofstream& file, const Light& l)
{
    file << "Component Light\n";
    file << "    Type " << static_cast<int>(l.type) << "\n";
    file << "    Color " << l.color.x << " " << l.color.y << " " << l.color.z << "\n";
    file << "    Intensity " << l.intensity << "\n";
    file << "    Range " << l.range << "\n";
    file << "    SpotAngle " << l.spotAngle << "\n";
}

static void SaveCollider(std::ofstream& file, const Collider& c)
{
    file << "Component Collider\n";
    file << "    Type " << static_cast<int>(c.type) << "\n";
    file << "    Offset " << c.offset.x << " " << c.offset.y << " " << c.offset.z << "\n";
    file << "    HalfExtents " << c.halfExtents.x << " " << c.halfExtents.y << " " << c.halfExtents.z << "\n";
    file << "    Radius " << c.radius << "\n";
    file << "    IsTrigger " << (c.isTrigger ? 1 : 0) << "\n";
}

static void SaveMaterial(std::ofstream& file, const Material& m)
{
    file << "Component Material\n";
    file << "    MaterialID " << m.materialID << "\n";
    file << "    Albedo " << m.albedo.x << " " << m.albedo.y << " " << m.albedo.z << "\n";
    file << "    Roughness " << m.roughness << "\n";
    file << "    Metallic " << m.metallic << "\n";
    file << "    AlbedoTex " << m.albedoTextureID << "\n";
    file << "    NormalTex " << m.normalTextureID << "\n";
    file << "    MetallicTex " << m.metallicTextureID << "\n";
    file << "    RoughnessTex " << m.roughnessTextureID << "\n";
    file << "    PipelineID " << m.pipelineID << "\n";
}

static void SaveMesh(std::ofstream& file, const ECS::Mesh& m)
{
    file << "Component Mesh\n";
    file << "    MeshID " << m.meshID << "\n";
    file << "    IndexCount " << m.indexCount << "\n";
    file << "    MaterialID " << m.materialID << "\n";
    file << "    BoundsMin " << m.boundsMin.x << " " << m.boundsMin.y << " " << m.boundsMin.z << "\n";
    file << "    BoundsMax " << m.boundsMax.x << " " << m.boundsMax.y << " " << m.boundsMax.z << "\n";
}

static void SaveAnimation(std::ofstream& file, const Animation& a)
{
    file << "Component Animation\n";
    file << "    State " << a.state << "\n";
    file << "    Time " << a.time << "\n";
    file << "    Speed " << a.speed << "\n";
    file << "    Loop " << (a.loop ? 1 : 0) << "\n";
    file << "    ClipEntity " << a.clipEntity.index << " " << a.clipEntity.generation << "\n";
}

void SceneSerializer::SaveScene(Scene& scene, const std::string& path)
{
    std::ofstream file(path);
    if (!file.is_open())
        return;

    ComponentManager& components = scene.GetComponents();

    file << "Scene\n\n";

    std::vector<Entity> entities = scene.GetAllEntities();

    for (Entity e : entities)
    {
        const Hierarchy* h = components.GetHierarchy(e);
        if (!h) continue;

        file << "Entity " << e.index << " " << e.generation << "\n";
        file << "Parent " << h->parent.index << " " << h->parent.generation << "\n";
        file << "FirstChild " << h->firstChild.index << " " << h->firstChild.generation << "\n";
        file << "NextSibling " << h->nextSibling.index << " " << h->nextSibling.generation << "\n";

        if (components.HasTransform(e))
            SaveTransform(file, *components.GetTransform(e));

        if (components.HasVelocity(e))
            SaveVelocity(file, *components.GetVelocity(e));

        if (components.HasHealth(e))
            SaveHealth(file, *components.GetHealth(e));

        if (components.HasLight(e))
            SaveLight(file, *components.GetLight(e));

        if (components.HasCollider(e))
            SaveCollider(file, *components.GetCollider(e));

        if (components.HasMaterial(e))
            SaveMaterial(file, *components.GetMaterial(e));

        if (components.HasMesh(e))
            SaveMesh(file, *components.GetMesh(e));

        if (components.HasAnimation(e))
            SaveAnimation(file, *components.GetAnimation(e));

        file << "---\n\n";
    }
}

// --- Load helpers ---

static void LoadTransform(std::ifstream& file, Transform& t)
{
    std::string line, token;
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> t.position.x >> t.position.y >> t.position.z; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> t.rotation.x >> t.rotation.y >> t.rotation.z; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> t.scale.x >> t.scale.y >> t.scale.z; }
}

static void LoadVelocity(std::ifstream& file, Velocity& v)
{
    std::string line, token;
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> v.linear.x >> v.linear.y >> v.linear.z; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> v.angular.x >> v.angular.y >> v.angular.z; }
}

static void LoadHealth(std::ifstream& file, Health& h)
{
    std::string line, token;
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> h.current; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> h.max; }
}

static void LoadLight(std::ifstream& file, Light& l)
{
    std::string line, token;
    std::getline(file, line);
    { std::istringstream s(line); int t; s >> token; s >> t; l.type = static_cast<LightType>(t); }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> l.color.x >> l.color.y >> l.color.z; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> l.intensity; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> l.range; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> l.spotAngle; }
}

static void LoadCollider(std::ifstream& file, Collider& c)
{
    std::string line, token;
    std::getline(file, line);
    { std::istringstream s(line); int t; s >> token; s >> t; c.type = static_cast<ColliderType>(t); }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> c.offset.x >> c.offset.y >> c.offset.z; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> c.halfExtents.x >> c.halfExtents.y >> c.halfExtents.z; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> c.radius; }
    std::getline(file, line);
    { std::istringstream s(line); int trig; s >> token; s >> trig; c.isTrigger = (trig != 0); }
}

static void LoadMaterial(std::ifstream& file, Material& m)
{
    std::string line, token;
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> m.materialID; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> m.albedo.x >> m.albedo.y >> m.albedo.z; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> m.roughness; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> m.metallic; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> m.albedoTextureID; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> m.normalTextureID; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> m.metallicTextureID; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> m.roughnessTextureID; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> m.pipelineID; }
}

static void LoadMesh(std::ifstream& file, ECS::Mesh& m)
{
    std::string line, token;
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> m.meshID; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> m.indexCount; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> m.materialID; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> m.boundsMin.x >> m.boundsMin.y >> m.boundsMin.z; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> m.boundsMax.x >> m.boundsMax.y >> m.boundsMax.z; }
}

static void LoadAnimation(std::ifstream& file, Animation& a)
{
    std::string line, token;
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> a.state; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> a.time; }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> a.speed; }
    std::getline(file, line);
    { std::istringstream s(line); int lp; s >> token; s >> lp; a.loop = (lp != 0); }
    std::getline(file, line);
    { std::istringstream s(line); s >> token; s >> a.clipEntity.index >> a.clipEntity.generation; }
}

void SceneSerializer::LoadScene(Scene& scene, const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return;

    ComponentManager& components = scene.GetComponents();

    scene.DestroyAllEntities();

    std::string line, token;

    std::getline(file, line); // "Scene"

    std::unordered_map<Entity, Entity> entityMap;
    std::vector<std::pair<Entity, Hierarchy>> hierData;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::istringstream iss(line);
        iss >> token;

        if (token == "Entity")
        {
            Entity saved;
            iss >> saved.index >> saved.generation;

            Entity e = scene.CreateEntity();
            entityMap[saved] = e;

            Hierarchy h{};
            std::getline(file, line);
            { std::istringstream p(line); p >> token; p >> h.parent.index >> h.parent.generation; }
            std::getline(file, line);
            { std::istringstream p(line); p >> token; p >> h.firstChild.index >> h.firstChild.generation; }
            std::getline(file, line);
            { std::istringstream p(line); p >> token; p >> h.nextSibling.index >> h.nextSibling.generation; }

            hierData.emplace_back(saved, h);

            while (std::getline(file, line))
            {
                if (line.empty())
                    continue;

                std::istringstream c(line);
                c >> token;

                if (token == "---")
                    break;

                if (token == "Component")
                {
                    std::string type;
                    c >> type;

                    if (type == "Transform")
                    {
                        Transform t;
                        LoadTransform(file, t);
                        components.AddTransform(e, t);
                    }
                    else if (type == "Velocity")
                    {
                        Velocity v;
                        LoadVelocity(file, v);
                        components.AddVelocity(e, v);
                    }
                    else if (type == "Health")
                    {
                        Health hcomp;
                        LoadHealth(file, hcomp);
                        components.AddHealth(e, hcomp);
                    }
                    else if (type == "Light")
                    {
                        Light l;
                        LoadLight(file, l);
                        components.AddLight(e, l);
                    }
                    else if (type == "Collider")
                    {
                        Collider ccol;
                        LoadCollider(file, ccol);
                        components.AddCollider(e, ccol);
                    }
                    else if (type == "Material")
                    {
                        Material m;
                        LoadMaterial(file, m);
                        components.AddMaterial(e, m);
                    }
                    else if (type == "Mesh")
                    {
                        ECS::Mesh m;
                        LoadMesh(file, m);
                        components.AddMesh(e, m);
                    }
                    else if (type == "Animation")
                    {
                        Animation a;
                        LoadAnimation(file, a);

                        if (a.clipEntity.IsValid() && entityMap.count(a.clipEntity))
                            a.clipEntity = entityMap[a.clipEntity];
                        else
                            a.clipEntity = INVALID_ENTITY;

                        components.AddAnimation(e, a);
                    }
                }
            }
        }
    }

    for (auto& [saved, h] : hierData)
    {
        Entity e = entityMap[saved];

        Hierarchy real{};

        if (h.parent.IsValid() && entityMap.count(h.parent))
            real.parent = entityMap[h.parent];
        else
            real.parent = INVALID_ENTITY;

        if (h.firstChild.IsValid() && entityMap.count(h.firstChild))
            real.firstChild = entityMap[h.firstChild];
        else
            real.firstChild = INVALID_ENTITY;

        if (h.nextSibling.IsValid() && entityMap.count(h.nextSibling))
            real.nextSibling = entityMap[h.nextSibling];
        else
            real.nextSibling = INVALID_ENTITY;

        components.AddHierarchy(e, real);
    }
}