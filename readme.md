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
-   焼き鈍し
    -   伸びず むしろ下がった
-   lightsout
    -   綺麗な性質が消えてる
        -   F_2でなくZでかつ解に0/1制約となるので
    -   上の段を決めて残りを一意に
        -   一意にならない
    -   線形代数
        -   同様にだめ
    -   整数計画問題に落とす
        -   できそう
-   beam search
    -   上から消してく部分で使えるかも
-   Test Case Generation
    1.  各cellに確率pK ([0.1, 0.8)で一様) でknightsを配置
    2.  boardを計算
    3.  各cellに確率pC ([0.1, 0.5)で一様) で\pm maxC (clamped) の範囲でずらす (maxC は { 1, 2, 3 } から一様)
    4.  nRand個 ([0, S)で一様) のcellsを[0, 9)から変更
    -   pC maxC が小さければいけそう
