function load()
	print("load "..entity.id)
end

function update()
	print("update "..entity.id)
	requestTransform(entity.id)
    if not inputStatePrev.mouseButton1 and inputStateCurr.mouseButton1 then
        entity.transform.y = -entity.transform.y
    end
    if (inputStateCurr.keys[4 + 1]>0) then
        entity.transform.rz = entity.transform.rz - 0.01
    end
    if (inputStateCurr.keys[7 + 1]>0) then
        entity.transform.rz = entity.transform.rz + 0.01
    end
    if (inputStateCurr.keys[22 + 1]>0) then
        entity.transform.ry = entity.transform.ry - 0.01
    end
    if (inputStateCurr.keys[26 + 1]>0) then
        entity.transform.ry = entity.transform.ry + 0.01
    end
end