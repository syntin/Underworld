//DB
#include "prefabSerializer.h"
#include <fstream>
#include <sstream>

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

void PrefabSerializer::SavePrefab(const Prefab& prefab, const std::string& path)
{
	std::ofstream file(path);
	if (!file.is_open())
		return;

	file << "Prefab\n";
	file << "Root " << prefab.rootTemplate.index << " " << prefab.rootTemplate.generation << "\n\n";

	for (auto& [templateEntity, pe] : prefab.entities)
	{
		file << "Entity " << templateEntity.index << " " << templateEntity.generation << "\n";

		file << "Parent " << pe.hierarchy.parent.index << " " << pe.hierarchy.parent.generation << "\n";
		file << "FirstChild " << pe.hierarchy.firstChild.index << " " << pe.hierarchy.firstChild.generation << "\n";
		file << "NextSibling " << pe.hierarchy.nextSibling.index << " " << pe.hierarchy.nextSibling.generation << "\n";

		for (auto& [name, comp] : pe.components)
		{
			if (std::holds_alternative<Transform>(comp))
				SaveTransform(file, std::get<Transform>(comp));
			else if (std::holds_alternative<Velocity>(comp))
				SaveVelocity(file, std::get<Velocity>(comp));
			else if (std::holds_alternative<Health>(comp))
				SaveHealth(file, std::get<Health>(comp));
			else if (std::holds_alternative<Light>(comp))
				SaveLight(file, std::get<Light>(comp));
			else if (std::holds_alternative<Collider>(comp))
				SaveCollider(file, std::get<Collider>(comp));
			else if (std::holds_alternative<Material>(comp))
				SaveMaterial(file, std::get<Material>(comp));
			else if (std::holds_alternative<ECS::Mesh>(comp))
				SaveMesh(file, std::get<ECS::Mesh>(comp));
			else if (std::holds_alternative<Animation>(comp))
				SaveAnimation(file, std::get<Animation>(comp));
		}

		file << "---\n\n";
	}
}

Prefab PrefabSerializer::LoadPrefab(const std::string& path)
{
    std::ifstream file(path);
    Prefab prefab;

    if (!file.is_open())
        return prefab;

    std::string line;
    std::string token;

    // "Prefab"
    std::getline(file, line);

    // "Root index generation"
    std::getline(file, line);
    {
        std::istringstream iss(line);
        iss >> token; // Root
        iss >> prefab.rootTemplate.index >> prefab.rootTemplate.generation;
    }

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::istringstream iss(line);
        iss >> token;

        if (token == "Entity")
        {
            Entity templateEntity;
            iss >> templateEntity.index >> templateEntity.generation;

            PrefabEntity pe;

            // Parent
            std::getline(file, line);
            {
                std::istringstream p(line);
                p >> token;
                p >> pe.hierarchy.parent.index >> pe.hierarchy.parent.generation;
            }

            // FirstChild
            std::getline(file, line);
            {
                std::istringstream fc(line);
                fc >> token;
                fc >> pe.hierarchy.firstChild.index >> pe.hierarchy.firstChild.generation;
            }

            // NextSibling
            std::getline(file, line);
            {
                std::istringstream ns(line);
                ns >> token;
                ns >> pe.hierarchy.nextSibling.index >> pe.hierarchy.nextSibling.generation;
            }

            // Components until "---"
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
                        std::getline(file, line); // Position
                        {
                            std::istringstream s(line);
                            s >> token; // Position
                            s >> t.position.x >> t.position.y >> t.position.z;
                        }
                        std::getline(file, line); // Rotation
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> t.rotation.x >> t.rotation.y >> t.rotation.z;
                        }
                        std::getline(file, line); // Scale
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> t.scale.x >> t.scale.y >> t.scale.z;
                        }
                        pe.components["Transform"] = t;
                    }
                    else if (type == "Velocity")
                    {
                        Velocity v;
                        std::getline(file, line); // Linear
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> v.linear.x >> v.linear.y >> v.linear.z;
                        }
                        std::getline(file, line); // Angular
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> v.angular.x >> v.angular.y >> v.angular.z;
                        }
                        pe.components["Velocity"] = v;
                    }
                    else if (type == "Health")
                    {
                        Health h;
                        std::getline(file, line); // Current
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> h.current;
                        }
                        std::getline(file, line); // Max
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> h.max;
                        }
                        pe.components["Health"] = h;
                    }
                    else if (type == "Light")
                    {
                        Light l;
                        std::getline(file, line); // Type
                        {
                            std::istringstream s(line);
                            int t;
                            s >> token;
                            s >> t;
                            l.type = static_cast<LightType>(t);
                        }
                        std::getline(file, line); // Color
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> l.color.x >> l.color.y >> l.color.z;
                        }
                        std::getline(file, line); // Intensity
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> l.intensity;
                        }
                        std::getline(file, line); // Range
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> l.range;
                        }
                        std::getline(file, line); // SpotAngle
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> l.spotAngle;
                        }
                        pe.components["Light"] = l;
                    }
                    else if (type == "Collider")
                    {
                        Collider ccol;
                        std::getline(file, line); // Type
                        {
                            std::istringstream s(line);
                            int t;
                            s >> token;
                            s >> t;
                            ccol.type = static_cast<ColliderType>(t);
                        }
                        std::getline(file, line); // Offset
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> ccol.offset.x >> ccol.offset.y >> ccol.offset.z;
                        }
                        std::getline(file, line); // HalfExtents
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> ccol.halfExtents.x >> ccol.halfExtents.y >> ccol.halfExtents.z;
                        }
                        std::getline(file, line); // Radius
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> ccol.radius;
                        }
                        std::getline(file, line); // IsTrigger
                        {
                            std::istringstream s(line);
                            int trig;
                            s >> token;
                            s >> trig;
                            ccol.isTrigger = (trig != 0);
                        }
                        pe.components["Collider"] = ccol;
                    }
                    else if (type == "Material")
                    {
                        Material m;
                        std::getline(file, line); // MaterialID
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> m.materialID;
                        }
                        std::getline(file, line); // Albedo
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> m.albedo.x >> m.albedo.y >> m.albedo.z;
                        }
                        std::getline(file, line); // Roughness
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> m.roughness;
                        }
                        std::getline(file, line); // Metallic
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> m.metallic;
                        }
                        std::getline(file, line); // AlbedoTex
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> m.albedoTextureID;
                        }
                        std::getline(file, line); // NormalTex
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> m.normalTextureID;
                        }
                        std::getline(file, line); // MetallicTex
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> m.metallicTextureID;
                        }
                        std::getline(file, line); // RoughnessTex
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> m.roughnessTextureID;
                        }
                        std::getline(file, line); // PipelineID
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> m.pipelineID;
                        }
                        pe.components["Material"] = m;
                    }
                    else if (type == "Mesh")
                    {
                        ECS::Mesh m;
                        std::getline(file, line); // MeshID
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> m.meshID;
                        }
                        std::getline(file, line); // IndexCount
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> m.indexCount;
                        }
                        std::getline(file, line); // MaterialID
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> m.materialID;
                        }
                        std::getline(file, line); // BoundsMin
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> m.boundsMin.x >> m.boundsMin.y >> m.boundsMin.z;
                        }
                        std::getline(file, line); // BoundsMax
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> m.boundsMax.x >> m.boundsMax.y >> m.boundsMax.z;
                        }
                        pe.components["Mesh"] = m;
                    }
                    else if (type == "Animation")
                    {
                        Animation a;
                        std::getline(file, line); // State
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> a.state;
                        }
                        std::getline(file, line); // Time
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> a.time;
                        }
                        std::getline(file, line); // Speed
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> a.speed;
                        }
                        std::getline(file, line); // Loop
                        {
                            std::istringstream s(line);
                            int lp;
                            s >> token;
                            s >> lp;
                            a.loop = (lp != 0);
                        }
                        std::getline(file, line); // ClipEntity
                        {
                            std::istringstream s(line);
                            s >> token;
                            s >> a.clipEntity.index >> a.clipEntity.generation;
                        }
                        pe.components["Animation"] = a;
                    }
                }
            }

            prefab.entities[templateEntity] = pe;
        }
    }

    return prefab;
}