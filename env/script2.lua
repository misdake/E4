function load()
	print("load "..entity.id)
end

function update()
	-- print("update "..entity.id)
	requestTransform(entity.id)

    if (inputStateCurr.keys[4 + 1]>0) then
        entity.transform.rz = entity.transform.rz - dt
    end
    if (inputStateCurr.keys[7 + 1]>0) then
        entity.transform.rz = entity.transform.rz + dt
    end
    if (inputStateCurr.keys[22 + 1]>0) then
        entity.transform.ry = entity.transform.ry - dt
    end
    if (inputStateCurr.keys[26 + 1]>0) then
        entity.transform.ry = entity.transform.ry + dt
    end
end