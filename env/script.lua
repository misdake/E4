function load()
    print("load")
end

function update()
	requireComponent(ComponentTransform)
    if not inputStatePrev.mouseButton1 and inputStateCurr.mouseButton1 then
        transform.y = -transform.y
    end
    if (inputStatePrev.keys[6]==0) and (inputStateCurr.keys[6]>0) then
        transform.x = transform.x + 0.2
    end
end