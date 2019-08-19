tiles = { "red", "blue", "yellow", "green" }
tileCount = 4

setLogFilter("texture")

function load()
    entity.list = {}
    entity.count = 0
    entity.transform.sx = screenHeight / screenWidth
    entity.tileIndex = 0
end

function update()
    if mouse1down() then
        local e = newEntityNameParent("name_" .. entity.count, entity.index)
        local transform = createTransform(e)
        transform.x = (inputStateCurr.mouseX / screenWidth * 2.0 - 1.0) * screenWidth / screenHeight
        transform.y = 1.0 - inputStateCurr.mouseY / screenHeight * 2.0
        transform.sx = 0.2
        transform.sy = 0.2
        transform.sz = 0.2
        local drawable = createDrawable(e)
        drawable.mesh = newMesh("builtin:plane")
        drawable.material = newMaterialTexture("sprites.txt")
        setMaterialTextureTile(drawable.material, tiles[entity.tileIndex + 1])
        entity.tileIndex = math.fmod(entity.tileIndex + 1, tileCount)

        entity.list[entity.count] = e
        entity.count = entity.count + 1
    end
    if mouse2down() then
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
                    print(entities[e].name)
                    --toggle visibility
                    entities[e].drawable.visible = not entities[e].drawable.visible
                end
            end
        end
    end
    if mouse3down() then
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
    if keydown(30) then
        local e = findEntityByName("name_" .. 1)
        if (e > 0) then
            print(e)
            entities[e].drawable.visible = not entities[e].drawable.visible
        end
    end
end