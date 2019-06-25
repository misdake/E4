function load()
    entity.time = 0
    entity.transform.sx = 0.3
    entity.transform.sy = 0.3
    entity.transform.sz = 0.3
    entity.transform.y = -0.5

    local e1 = newEntityFromFile("Male_LookingUp.obj")
    entities[e1].transform.x = -3
    entities[e1].transform.ry = 1
    entities[e1].transform.parent = entity.index
    local e2 = newEntityFromFile("Male_LookingUp.obj")
    entities[e2].transform.x = 0
    entities[e2].transform.ry = 2
    entities[e2].transform.parent = entity.index
    local e3 = newEntityFromFile("Male_LookingUp.obj")
    entities[e3].transform.x = 3
    entities[e3].transform.ry = 3
    entities[e3].transform.parent = entity.index

    local lightEntity = newEntity()
    createTransform(lightEntity)
    entities[lightEntity].transform.x = 1
    entities[lightEntity].transform.y = 0.5
    entities[lightEntity].transform.sx = 0.1
    entities[lightEntity].transform.sy = 0.1
    entities[lightEntity].transform.sz = 0.1
    createEnv(lightEntity, "FFFFFFFF")
    entity.l = lightEntity;
    local drawable = createDrawable(lightEntity)
    drawable.mesh = newMesh("builtin:plane")
    drawable.material = newMaterialTexture("street.jpg")
end

function update()
    entity.time = entity.time + dt;
    local rad = math.fmod(entity.time, math.pi * 2);
    entities[entity.l].transform.x = math.sin(rad) * 0.8
    entities[entity.l].transform.z = math.cos(rad) * 0.8
end