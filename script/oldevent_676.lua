--function oldevent_676()
     if instruct_50(138,139,140,141,142,6,0)==false then   --  50(32):新指令，暂时没有解释
        instruct_1(2481,75,0);   --  1(1):[???]说: 想要”玉笛谁家听落梅”，*就赶紧将羊羔坐臀，小猪耳*朵，小牛腰子，獐腿肉，兔*肉这五种材料找来．
        instruct_0();   --  0(0)::空语句(清屏)
        do return; end
    end
    instruct_32(138,-1);   --  32(20):物品[羊羔坐臀]+[-1]
    instruct_32(139,-1);   --  32(20):物品[小牛腰子]+[-1]
    instruct_32(140,-1);   --  32(20):物品[小猪耳朵]+[-1]
    instruct_32(141,-1);   --  32(20):物品[獐腿肉]+[-1]
    instruct_32(142,-1);   --  32(20):物品[兔肉]+[-1]
    instruct_1(2482,75,0);   --  1(1):[???]说: 好，材料通通都有了，我马*上就把”玉笛谁家听落梅”*作给你．
    instruct_0();   --  0(0)::空语句(清屏)
    instruct_14();   --  14(E):场景变黑
    instruct_3(-2,0,0,0,-1,-1,-1,-1,-1,-1,-2,-2,-2);   --  3(3):修改事件定义:当前场景:场景事件编号 [0]
    instruct_3(-2,1,0,0,-1,-1,-1,-1,-1,-1,-2,-2,-2);   --  3(3):修改事件定义:当前场景:场景事件编号 [1]
    instruct_3(-2,2,1,1,-1,-1,-1,7536,7536,7536,-2,-2,-2);   --  3(3):修改事件定义:当前场景:场景事件编号 [2]
    instruct_3(-2,3,1,1,-1,-1,-1,7580,7580,7580,-2,-2,-2);   --  3(3):修改事件定义:当前场景:场景事件编号 [3]
    instruct_40(0);   --  40(28):改变主角站立方向0
    instruct_0();   --  0(0)::空语句(清屏)
    instruct_13();   --  13(D):重新显示场景
    instruct_44(2,7536,7578,3,7580,7622);   --  44(2C):显示动画
    instruct_44(2,7536,7578,3,7580,7622);   --  44(2C):显示动画
    instruct_44(2,7536,7578,3,7580,7622);   --  44(2C):显示动画
    instruct_14();   --  14(E):场景变黑
    instruct_3(-2,2,1,1,-1,-1,-1,2718,2718,2718,-2,-2,-2);   --  3(3):修改事件定义:当前场景:场景事件编号 [2]
    instruct_3(-2,3,1,1,-1,-1,-1,2720,2720,2720,-2,-2,-2);   --  3(3):修改事件定义:当前场景:场景事件编号 [3]
    instruct_3(-2,5,1,1,689,-1,-1,5100,5100,5100,-2,-2,-2);   --  3(3):修改事件定义:当前场景:场景事件编号 [5]
    instruct_3(-2,4,-2,-2,-1,-1,677,-2,-2,-2,-2,-2,-2);   --  3(3):修改事件定义:当前场景:场景事件编号 [4]
    instruct_0();   --  0(0)::空语句(清屏)
    instruct_13();   --  13(D):重新显示场景
    instruct_1(2483,75,0);   --  1(1):[???]说: 好了，完成了，*这一道”玉笛谁家听落梅”*你拿去吧．
    instruct_0();   --  0(0)::空语句(清屏)
    instruct_1(2484,0,1);   --  1(1):[WWW]说: 谢谢林师父．
    instruct_0();   --  0(0)::空语句(清屏)
    instruct_2(176,1);   --  2(2):得到物品[玉笛谁家听落梅][1]
--end

