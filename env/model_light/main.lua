setLogFilter("model")

function load()
    entity.time = 0
    entity.transform.sx = 0.3
    entity.transform.sy = 0.3
    entity.transform.sz = 0.3
    entity.transform.y = -0.5

    entity.cameraFov = math.pi / 4
    entity.lastWheel = 0

    local cameraEntity1 = newEntity()
    local cameraTransform1 = createTransform(cameraEntity1)
    cameraTransform1.x = 2
    cameraTransform1.y = 0
    cameraTransform1.z = 3
    enableCamera(cameraEntity1, CameraType.PROJ, entity.cameraFov)
    local cameraEntity2 = newEntity()
    local cameraTransform2 = createTransform(cameraEntity2)
    cameraTransform2.x = -2
    cameraTransform2.y = 0
    cameraTransform2.z = 3
    enableCamera(cameraEntity2, CameraType.PROJ, entity.cameraFov)

    entity.cameraEntity1 = cameraEntity1
    entity.cameraEntity2 = cameraEntity2

    local e1 = newEntityFromFileParent("Male_LookingUp.obj", entity.index)
    entities[e1].transform.x = -3
    entities[e1].transform.ry = 1
    local e2 = newEntityFromFileParent("Male_LookingUp.obj", entity.index)
    entities[e2].transform.x = 0
    entities[e2].transform.ry = 2
    local e3 = newEntityFromFileParent("Male_LookingUp.obj", entity.index)
    entities[e3].transform.x = 3
    entities[e3].transform.ry = 3

    local lightEntity = newEntity()
    createTransform(lightEntity)
    entities[lightEntity].transform.x = 1
    entities[lightEntity].transform.y = 0.5
    entities[lightEntity].transform.sx = 0.1
    entities[lightEntity].transform.sy = 0.1
    entities[lightEntity].transform.sz = 0.1
    enableLight(lightEntity, LightType.POINT, "FF404040", "FFFFFFFF", "FFFFFFFF")
    entity.l = lightEntity;
    local drawable = createDrawable(lightEntity)
    drawable.mesh = newMesh("builtin:plane")
    drawable.material = newMaterialTexture("street.jpg")
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
    entity.time = entity.time + dt;
    local rad = math.fmod(entity.time, math.pi * 2);
    if mouse1down() then
        entity.mouseX = inputStateCurr.mouseX
        entity.mouseY = inputStateCurr.mouseY
    end
    if inputStatePrev.mouseButton1 and inputStateCurr.mouseButton1 then
        local t1 = entities[entity.cameraEntity1].transform
        local t2 = entities[entity.cameraEntity2].transform
        t1.ry = t1.ry + (inputStateCurr.mouseX - inputStatePrev.mouseX) * math.pi / 1000
        t1.rx = t1.rx + (inputStateCurr.mouseY - inputStatePrev.mouseY) * math.pi / 1000
        t2.ry = t1.ry
        t2.rx = t1.rx
        entity.mouseX = inputStateCurr.mouseX
        entity.mouseY = inputStateCurr.mouseY
    end

    if keydown(30) then
        setActiveCamera(entity.cameraEntity1)
    end
    if keydown(31) then
        setActiveCamera(entity.cameraEntity2)
    end

    if entity.lastWheel ~= inputStateCurr.wheelY then
        enableCamera(entity.cameraEntity1, CameraType.PROJ, entity.cameraFov + inputStateCurr.wheelY * 0.1);
        enableCamera(entity.cameraEntity2, CameraType.PROJ, entity.cameraFov + inputStateCurr.wheelY * 0.1);
        entity.lastWheel = inputStateCurr.wheelY
    end

    entities[entity.l].transform.x = math.sin(rad) * 0.8
    entities[entity.l].transform.z = math.cos(rad) * 0.8
end