function load()
    entity.transform.sx = 0.3
    entity.transform.sy = 0.3
    entity.transform.sz = 0.3
    entity.transform.y = -0.5

    local e = newEntityFromFile("Male_LookingUp.obj")
    entities[e].transform.parent = entity.index

    local lightEntity = newEntity()
    createTransform(lightEntity)
    createEnv(lightEntity, "FFFFFFFF")
    entity.l = lightEntity;
end

function update()
    entities[entity.l].transform.ry = math.fmod(entities[entity.l].transform.ry + 1 * dt, math.pi * 2)
    entity.transform.ry = math.fmod(entity.transform.ry + -0.2 * dt, math.pi * 2)
end