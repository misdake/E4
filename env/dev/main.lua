function load()
    entity.time = 0
    entity.transform.sx = 0.3
    entity.transform.sy = 0.3
    entity.transform.sz = 0.3
    entity.transform.y = -0.5

    local cameraEntity = newEntity()
    entity.cameraEntity = cameraEntity
    local cameraTransform = createTransform(cameraEntity)
    cameraTransform.z = 3
    enableCamera(cameraEntity, "PROJ", math.pi / 4)

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
    enableLight(lightEntity, "FF404040", "FFFFFFFF", "FFFFFFFF")
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
    if  (not inputStatePrev.mouseButton1) and inputStateCurr.mouseButton1 then
        entity.mouseX = inputStateCurr.mouseX
        entity.mouseY = inputStateCurr.mouseY
    end
    if inputStatePrev.mouseButton1 and inputStateCurr.mouseButton1 then
        local t = entities[entity.cameraEntity].transform
        t.ry = t.ry + (inputStateCurr.mouseX - inputStatePrev.mouseX) * math.pi / 1000
        t.rx = t.rx + (inputStateCurr.mouseY - inputStatePrev.mouseY) * math.pi / 1000
        entity.mouseX = inputStateCurr.mouseX
        entity.mouseY = inputStateCurr.mouseY
        print(t.ry)
    end
    if  (not inputStateCurr.mouseButton1) and inputStatePrev.mouseButton1 then

    end
    entities[entity.l].transform.x = math.sin(rad) * 0.8
    entities[entity.l].transform.z = math.cos(rad) * 0.8
end