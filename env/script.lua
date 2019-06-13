function load()
	print("load "..entity.id)
end

function update()
	print("update "..entity.id)
	requestTransform(entity.id)
    if not inputStatePrev.mouseButton1 and inputStateCurr.mouseButton1 then
        entity.transform.y = -entity.transform.y
    end

	entity.transform.y = entity.transform.y + (inputStateCurr.wheelY - inputStatePrev.wheelY) * 0.1

    if (inputStateCurr.keys[80 + 1]>0) then
        entity.transform.x = entity.transform.x - 0.005
    end
    if (inputStateCurr.keys[79 + 1]>0) then
        entity.transform.x = entity.transform.x + 0.005
    end
    if (inputStateCurr.keys[81 + 1]>0) then
        entity.transform.y = entity.transform.y - 0.005
    end
    if (inputStateCurr.keys[82 + 1]>0) then
        entity.transform.y = entity.transform.y + 0.005
    end
end