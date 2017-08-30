# TopCoder Marathon Match: Poland Lightning Round. KnightsAttacks

-   Problem Statement: <https://community.topcoder.com/longcontest/?module=ViewProblemStatement&rd=16981&pm=14679>
-   Standings: <https://community.topcoder.com/longcontest/?module=ViewStandings&rd=16981>
-   Submit: <https://community.topcoder.com/longcontest/?module=Submit&compid=57153&rd=16981&cd=15207>

## 記録

-   1点反転 山登り: SEED = 3 で Score = 97000 ぐらい
    -   提出 7709adf52b9093ecac6ba7bdc2bc20396a4ec3ad
        -   wleite 	991250.00 	08.29.2017 22:56:16
        -   kurenai3110 	671250.00 	08.29.2017 23:34:40
        -   kimiyuki 	400000.00 	08.30.2017 00:10:03
        -   EvbCFfp1XB 	335000.00 	08.29.2017 23:02:56
        -   LLI_E_P_JI_O_K 	102500.00 	08.29.2017 23:22:25
-   lightsout
    -   綺麗な性質が消えてる
        -   F_2でなくZでかつ解に0/1制約となるので
    -   上の段を決めて残りを一意に
        -   一意にならない
    -   線形代数
        -   同様にだめ
    -   整数計画問題に落とす
        -   できそう
        -   とりあえず線形緩和かな
        -   Karmarkar法そのままだと操作回数 O(n^3.5) らしい つまりO(s^7)
        -   疎なのでなんとかなるか それでも厳しそう
        -   flowにならないか
-   beam search
    -   上から消してく部分で使えるかも
-   Test Case Generation
    1.  各cellに確率pK ([0.1, 0.8)で一様) でknightsを配置
    2.  boardを計算
    3.  各cellに確率pC ([0.1, 0.5)で一様) で\pm maxC (clamped) の範囲でずらす (maxC は { 1, 2, 3 } から一様)
    4.  nRand個 ([0, S)で一様) のcellsを[0, 9)から変更
    -   pC maxC が小さければいけそう
-   焼き鈍し
    -   微増
    -   提出 e67d238c3cf9823d78a3f05bf90eb9740e357436
        -   kurenai3110 	973600.00 	08.30.2017 06:08:05
        -   kimiyuki 	941600.00 	08.30.2017 11:11:03
        -   wleite 	912000.00 	08.30.2017 00:56:41
        -   gorbunov 	846000.00 	08.30.2017 00:56:02
        -   EvbCFfp1XB 	832400.00 	08.30.2017 09:32:00
    -   まさかの2位
-   定数倍
    -   提出 2ec2dbba215bf7dca0a636b3a6d9878797bbbcd1
    -   4位から3位へ
    -   数ケースでTLEあるかも
-   近傍
    -   swap
        -   あんまり
    -   cellを選んで差を0に
        -   そうでもない
