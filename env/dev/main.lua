function load()
    entity.transform.sx = 0.3
    entity.transform.sy = 0.3
    entity.transform.sz = 0.3
    local drawable = createDrawable(entity.index)
    drawable.mesh = newMesh("file:Bush1.obj")
    drawable.material = newMaterialColor("FFFFFFFF")
    --drawable.material = newMaterialTexture("CarTexture.png")
end

function update()
    entity.transform.ry = math.fmod(entity.transform.ry + 1 * dt, math.pi * 2)
end