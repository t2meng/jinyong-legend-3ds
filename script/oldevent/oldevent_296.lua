--function oldevent_296()

    if instruct_16(36,6,0) ==false then    --  16(10):队伍是否有[林平之]是则跳转到:Label0
        instruct_1(1014,24,0);   --  1(1):[余沧海]说: 哼！*废话少说，要杀要剐随你．
        instruct_0();   --  0(0)::空语句(清屏)
        do return; end
    end    --:Label0

    instruct_1(1025,36,1);   --  1(1):[林平之]说: 余沧海，来受死吧，*今天我要你青城派在武林中*消失．
    instruct_0();   --  0(0)::空语句(清屏)

    if instruct_6(51,3,0,0) ==false then    --  6(6):战斗[51]是则跳转到:Label1
        instruct_15(83);   --  15(F):战斗失败，死亡
        do return; end
    end    --:Label1

    instruct_3(-2,0,0,0,-1,-1,-1,-1,-1,-1,-2,-2,-2);   --  3(3):修改事件定义:当前场景:场景事件编号 [0]
    instruct_3(-2,1,0,0,-1,-1,-1,-1,-1,-1,-2,-2,-2);   --  3(3):修改事件定义:当前场景:场景事件编号 [1]
    instruct_3(-2,2,0,0,-1,-1,-1,-1,-1,-1,-2,-2,-2);   --  3(3):修改事件定义:当前场景:场景事件编号 [2]
    instruct_3(-2,3,0,0,-1,-1,-1,-1,-1,-1,-2,-2,-2);   --  3(3):修改事件定义:当前场景:场景事件编号 [3]
    instruct_3(-2,4,0,0,-1,-1,-1,-1,-1,-1,-2,-2,-2);   --  3(3):修改事件定义:当前场景:场景事件编号 [4]
    instruct_0();   --  0(0)::空语句(清屏)
    instruct_13();   --  13(D):重新显示场景
    instruct_1(1026,36,1);   --  1(1):[林平之]说: 痛快！！！*今天真是杀的大快人心．
    instruct_0();   --  0(0)::空语句(清屏)
    instruct_56(3);   --  56(38):提高声望值3
--end

