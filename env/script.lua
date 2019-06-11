function load()
    print("load")
end

function update()
    if not frameState.inputStatePrev.mouseButton1 and frameState.inputStateCurr.mouseButton1 then
        print("mouse1 down")
    end
    --if not frameState.inputStatePrev.keys[5] and frameState.inputStateCurr.keys[5] then
    --    print("mouse1 down")
    --end
end