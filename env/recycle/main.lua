function load()
    entity.list = {}
    entity.count = 0
    entity.transform.sx = screenHeight / screenWidth
end

function mouse1()
    return (not inputStatePrev.mouseButton1) and inputStateCurr.mouseButton1
end
function mouse2()
    return (not inputStatePrev.mouseButton2) and inputStateCurr.mouseButton2
end
function mouse3()
    return (not inputStatePrev.mouseButton3) and inputStateCurr.mouseButton3
end

function update()
    if mouse1() then
        local e = newEntity()
        local transform = createTransform(e)
        transform.parent = entity.index
        transform.x = (inputStateCurr.mouseX / screenWidth * 2.0 - 1.0) * screenWidth / screenHeight
        transform.y = 1.0 - inputStateCurr.mouseY / screenHeight * 2.0
        transform.sx = 0.2
        transform.sy = 0.2
        transform.sz = 0.2
        local drawable = createDrawable(e)
        drawable.mesh = newMesh("builtin:plane")
        drawable.material = newMaterialTexture("footprint.png")

        entity.list[entity.count] = e
        entity.count = entity.count + 1
    end
    if mouse2() then
        --test intersection
        local mouseX = (inputStateCurr.mouseX / screenWidth * 2.0 - 1.0) * screenWidth / screenHeight
        local mouseY = 1.0 - inputStateCurr.mouseY / screenHeight * 2.0
        local sx = 0.1
        local sy = 0.1
        for i = 0, entity.count - 1 do
            local e = entity.list[i]
            if entities[e].transform then
                local transform = entities[e].transform
                local dx = transform.x - mouseX
                local dy = transform.y - mouseY
                if math.abs(dx) < sx and math.abs(dy) < sy then
                    --toggle visibility
                    entities[e].drawable.visible = not entities[e].drawable.visible
                end
            end
        end
    end
    if mouse3() then
        --test intersection
        local mouseX = (inputStateCurr.mouseX / screenWidth * 2.0 - 1.0) * screenWidth / screenHeight
        local mouseY = 1.0 - inputStateCurr.mouseY / screenHeight * 2.0
        local sx = 0.1
        local sy = 0.1
        for i = 0, entity.count - 1 do
            local e = entity.list[i]
            if entities[e].transform then
                local transform = entities[e].transform
                local dx = transform.x - mouseX
                local dy = transform.y - mouseY
                if math.abs(dx) < sx and math.abs(dy) < sy then
                    --remove entity
                    deleteEntity(e)
                end
            end
        end
    end
end