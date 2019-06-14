function createChild(x, y, size, textureName)
    local e = createEntity()
    local transform = createTransform(e)
    transform.x = x;
    transform.y = y;
    transform.z = 0;
    transform.sx = size;
    transform.sy = size;
    transform.parent = entity.id;
    local drawable = createDrawable(e)
    drawable.mesh = newMesh("builtin:plane")
    drawable.material = newMaterialTexture(textureName)
    return e
end

function gameStart()
    entity.y = 0.5
    entity.speed = 0
    entity.acc = -5
end

function load()
    requestTransform(entity.id)
    entity.transform.sx = screenHeight / screenWidth

    local e = createChild(-0.5, 0.5, 0.25, "flappybird.png")
    bird = e
    gameStart()
end

function update()
    requestTransform(bird)
    local birdTransform = entities[bird].transform
    if mouse1down() then
        entity.speed = 2
    end
    entity.speed = entity.speed + dt * entity.acc
    entity.y = entity.y + entity.speed * dt
    birdTransform.y = entity.y

    birdTransform.rz = math.atan(entity.speed / 3)

    if entity.y < -1 then
        gameStart()
    end
end