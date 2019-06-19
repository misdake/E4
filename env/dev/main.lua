function load()
	print("load "..entity.index)

    local drawable = createDrawable(entity.index)
    drawable.mesh = newMesh("builtin:plane")
    drawable.material = newMaterialTexture("street.jpg")
end

function update()
	-- print("update "..entity.index)

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
	    entity.drawable.material = newMaterialTexture("footprint.png")
    end
    if not inputStatePrev.mouseButton3 and inputStateCurr.mouseButton3 then
	    entity.drawable.material = newMaterialTexture("favicon.png")
    end

    -- if not inputStatePrev.mouseButton3 and inputStateCurr.mouseButton3 then
    --     local e = createEntity()
    --     local transform = createTransform(e)
    --     transform.x = inputStateCurr.mouseX / 500.0 * 2.0 - 1.0
    --     transform.y = 1.0 - inputStateCurr.mouseY / 500.0 * 2.0
    --     transform.sx = 0.1
    --     transform.sy = 0.1
    --     transform.sz = 0.1
    --     local drawable = createDrawable(e)
    --     drawable.mesh = newMesh("builtin:plane")
    --     drawable.material = newMaterialTexture("footprint.png")
    --     -- drawable.material = newMaterialColor("FF0080FF")
    --     createScript(e, "script1.lua")
    -- end
end