function load()
	print("load "..entity.id)
end

function update()
	-- print("update "..entity.id)
	requestTransform(entity.id)

    if not inputStatePrev.mouseButton1 and inputStateCurr.mouseButton1 then
        entity.x = inputStateCurr.mouseX
        entity.y = inputStateCurr.mouseY
    end

    if inputStateCurr.mouseButton1 then
        entity.transform.x = entity.transform.x + (inputStateCurr.mouseX - entity.x) * 1 * dt
        entity.transform.y = entity.transform.y + (inputStateCurr.mouseY - entity.y) * -1 * dt
        entity.x = inputStateCurr.mouseX
        entity.y = inputStateCurr.mouseY
    end

    if not inputStatePrev.mouseButton2 and inputStateCurr.mouseButton2 then
	    requestDrawable(entity.id)
	    entity.drawable.material = newMaterial("footprint.png")
    end

    if not inputStatePrev.mouseButton3 and inputStateCurr.mouseButton3 then
        local e = createEntity()
        local transform = createTransform(e)
        transform.x = inputStateCurr.mouseX / 500.0 * 2.0 - 1.0
        transform.y = 1.0 - inputStateCurr.mouseY / 500.0 * 2.0
        transform.sx = 0.1
        transform.sy = 0.1
        transform.sz = 0.1
        local drawable = createDrawable(e)
        drawable.mesh = newMesh("builtin:plane")
        drawable.material = newMaterial("footprint.png")
        createScript(e, "script1.lua")
    end
end