function load()
    entity.transform.sx = 0.3
    entity.transform.sy = 0.3
    entity.transform.sz = 0.3

    local e = newEntityFromFile("Male_LookingUp.obj")
    entities[e].transform.parent = entity.index
end

function update()
    entity.transform.ry = math.fmod(entity.transform.ry + 1 * dt, math.pi * 2)
end