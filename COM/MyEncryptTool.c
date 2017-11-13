/******************************************************************************************/
/*****************************************头文件*******************************************/

#include	"MyEncryptTool.h"
#include	"MyMem.h"
#include	"user_fatfs.h"


#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	"string.h"
/******************************************************************************************/
/*****************************************局部变量声明*************************************/
const char myKey[54] = "wuhannewcandoshengwukejigufenyouxiangongsidezhangxiong";

const char myWord[94][94] = 
{
	",!D@q]Pv+UFba`9p~/Je;W&|fY:\"8ms2NjuAE?56S[>Q.1Kn40G*Bd(x{OLrt#$7HCg'XcV\\}^R=Z%koM_ywThIi)-3<zl",
	"sq-[!lmEd\"0#I;,}_\\bze{6OnUjy^=]2C~NG?<4JgH7.A/hr1:@B9'SfX&PDpQ)>T*|kRMuxKioWZ8`LwV+%a(F3Ycvt5$",
	")^+I1Sb_=x%9caTM&>8etOG2\"E<d:f@VKLg;Y(o7`j/WP{lQn?45H-\\u3y0,Xw.J}Nmh~zrv[iUsC|'FBAR6Zk*p#!q$]D",
	"@u}Rxp/XPg|>AkvsJd?YI`(m192e]_q4VL~oNFt)B{;-<a[S73WC.$n'G5zrwK!:8%E&jD#UhM6,^Oc+THb0\"Qy*Zilf\\=",
	"cxgR?7,`Cw8krTlqZ\"Q.a<N31[6WyXFni|5B2L^P\\]d@{#e!~-m)pG=E/jt;sfD%A:}H94U+>OS$IKuzo(0M&Yvh_J'*bV",
	"<oB'Vl}kr*TC^Nba4?)h8etvMXG#-9F!|jy{&@m_%w\"H(KW]g10dnS\\2`uIp.7+~U=xY3sP$>Zq6ORz,i5:AJ;Ecf/QLD[",
	":~_G7#o\\/af[uL]WRs,;dq\"r4AN8{3KeTF*0b%Q9k!HcPpJO2+|wn-lZz>^)$jXy.t}M5DE@(1B?mxiIvY<=gS&6U`h'VC",
	"?_X89U7NsR&~qV/b`B[}nwOF-2\\@Tcdp'HIe{.v\"iJE=ZrWu>h0<m:t5g^Kl*GQa;Dx#j%)o,A6]3CkYy!4|zS$M1P+L(f",
	"P};C.Bu@WIv)b[8de5'Q|,gJ]zkpL_*y\\nG&lK`7#rYxMU%:m<2+V=>1\"qR(F!hs09-aX6NST$iZotD^OEjw3A4cH?f~{/",
	"(k]Q^qn92Y-K<tv3T0'CZr!)}s8,;uDOfg#6L>hJ:.U\"dz+G\\m=pM/xV{_Xo4SIyec15aE`?iB~*[l$&PNjAFR7Wbw|%@H",
	"<H[I(R5ajB\\Xodw#l{hYWmKTe70DFC8!cE6.}%_-rUZ@gPp'|9qk+S;Au)Mv$fJ2&iQ:,z=N1O?t`b/s]^\"nyL*V43~xG>",
	"MjV7l@h/^`LAg*+#[J3XtG:.Sq(Y=s0?|Bb-;ufdn>\"_aZr]1o,2%w&CiyQT'W\\eR4O5!{69U<PKHcNIp}~D)Ex$8kzmvF",
	"[awtqHF|V\"]gE\\k_J?&s{Qj/8Avzm=r9CUP4#(IS@i;'eWZKB7l3!<`y%^>,pXNf$:ou5-ML.G6chn*Obd1}T2D0Rx+)~Y",
	"?3uIHw/$hlA71Rp_{vs@cX9y.PFb)nNCT5*<}>YiDM:`m;Z-xU2|LV]!OK'[Q~faotej,6(rg0Sk+\"W%4G8dq=&z\\^JB#E",
	"?b@D1ai!<U$)-zGH;t`r+TdZhulNB5me~:|M{.7SKP2Es/,Cn^6oc\\0'LW>QA[vqpw(*}f_#8\"O3g%I4]y9=&kXRFxjJVY",
	"|FasvNOA_xfER.V<Sy~!5#)@Zg$i+,43Xq^>hBkU\\}dz6w*/{(`GW;2CntIp%:LoYmbJQuH9=P1[-'&Dr7?lMTe]\"j80Kc",
	"n.EkFg-1^_NDC+OxoRpZia[r<z,#~U:|wqljtuvK!G@4/y{$eTXLfB]MVSc)&H`J%W6Yd?*9(='QsA\\b5>P}82I0hm;37\"",
	"x%HdhJ[7]B0+{;|E\\RWYtKoT(53Dc.,&Z@~M^6:w8b\"2>*Se$A=Pn'<GQ_qOj!/s#fi-ma94)ulg`rXkpC1Lvy?}VFNzUI",
	"O5_M@6'0(?%wa4zij9e~|uPZA}$^>RJY[q-GUsN8:Tgr*1],hKXc#{HB!FI<+m7b\"D`;\\3Cf&k/LlvxtpVy)=2oESQdn.W",
	"a?6-dhL!1jVQ9r$8PbclW7XU<\"f_CNtF%{'Y4qO>m,D3[;wGvJ~K=s@oy2.xz5T|:Z`+pH}eMng/S#]0I\\^(i)RAkBu*&E",
	",C8}97(NtQhz\\Anp%sTM>[:xd~gkjH3aKVU=2eXc6E@#01Jrl-Z]?y{<4/)!Y*;$Gv.LR^IqS'W`FBP\"wD|u_iOo5&bm+f",
	"JTmcn(i8.7-UZo^AC6/2+u=:\\t}W1PSs&,FO)vhgq<G!aMzK>f3H\";9R@p`V*#wIl]|L0BEe[{?4x'd~%r$jD_XkbyYQN5",
	"3UB#Mwqj&QO`rY*I'uV<Lz8t,PZnNm+o90]\\hCEek1G%d[f{.R!F|\"-laJ5x2:p7b;Wv~)cy$DKsT}H>?(4ASgX^/6i@=_",
	"=}R7NCGdn[yW^,#4Yr6fhpwq.t3I?U@|&m*<j+F)DLx9v8H{ZJo2T1`V0sz\\Eak~>5-AliXMOKu\"eB(;'c%g]_S/PQ:$!b",
	"Cj98:d\"}qhN)F#X2x!-D0usTA`/rSV\\ZYIWw$=+v*QHc@.LR;KaM_gb?znf]o~5yG<,O7'%e1p{[E3t>|6(^4lBmPUJik&",
	"[YSpF.]Ugu$ky0Tz?f4nA;C7|)s~q1}-:=X!bd^E,/`wta>ZK6mNJLR8\"Hlc*j&xIPM(#D2<5+o'VOiB9_e%W{h3@QrGv\\",
	"}D-Ix(z4;X)m_kF0yj|VphCwc*!l$OtG8<5LE@3ifHaW^vY/{U7TobsJeq#Q\"'.6>AnR?9PZ:,uM`&g=2[drSKN~+B\\%]1",
	"1|:s-RuXc>}r#LY]Tl=BAe0\\6g&3/Mj`ofi)hxzOvb_k<$.qZw+n'atPEQ?y[2HWG;@mpNS9FDU8dK7%!V\"J~,{C5(4I*^",
	"HP{;d\"iv7oh~Qa2fDUERMAB:[glY(`p$uCy?6+r/&_]GWjZJ}5-n4#|<\\)XL9*S8t!szk'e1.03OF=m^Ncq@bw,KTV%>xI",
	"6PO3n@aIWbQ{%,[RUDz'oj>Mt!f=`/:*i_&m1Bx2KJ?H$|\"4d]kZgs<c\\)y8hl~XpSNue5GTLA;YV^r0F9C+(}-vw.q#E7",
	"xgn3RK%6=G10:I~ClWe@B{a<)_rq;Q+7F#iS/2O^4(8$Y]Ew!LT>Vj\"ZJ*uk-f5tHp`bh|y\\s'9c,dX.oUmN&A}PMzv?D[",
	"%U{;&/kf0`q|<8ODiMgH5_AhJv9oPWY*.x~!:b,tF^$=RZ'VK\\Eads2T6l4c1jN)rpy}7>[#-Bmu@(Q3SGzL?C+neX]\"Iw",
	"9ZSB}gqM.XI3sA;+G^R|\"$_D[/rh5?6dNe@kyT,zj:J'Clv`fato(KPm=\\n>]cF8ix4LQ-72w<V~1{YEOWp)!U#H&b%u0*",
	"qL7v90=+ki#-u>M'dzT4r!%y|[}t)mx<AhZ^\\_VG@HXY?2*5$&R:pCbeEO]1~S/;.JcoW(Qf63j8`IDUn,{FB\"lsPwNKag",
	"NpF]sj(i%B*bUkL9_$yo;4vTI~nY=wchf,mA6S3>@8!gJ<r^-/W2xdVG5+ZO)\\\"u#.Ml[QD1EX0Ctz7H?K{P:aeR&|`'q}",
	"r+k2q\\lML->w38}nP!zHZ'hWKf1iX/o40{B7#:uEVN$I]S.OJseQG=R?jaY6<*U`Tx5_m[9yt)A;gb^\"d@Cv&pD|(~F,c%",
	"*E!'1U:xkh|<B{wR$bDsG%ZT3cFz,y>odj.@Jp+XgP4[}=9L0Mm(`SH\"/\\^q#WQ6?rtAl]aIYNv_~-5K;VeufnC&2)i78O",
	"h'{WPv<?/K6i,dGR1b3s*Hag|k2FB&QT_Cp`;.YJeM]c=(%9t7)oN\\->L8Vq$rD~jE#!0U@}Xwy+m\"Z^lOSzxf4InuA5[:",
	"oJI)p<xs,2_+E}T=gF*DU?.M/]3;LhYmV9>WtH%n~-#($|8frN\\[GXR{i:'^d\"Seklv56A7Cc@a10O&`K!bzBjqyZuw4QP",
	"%nv];>\"oDLdMV}0<H#a5sK8&ElYQJ\\O9rz@({[ceB3|6hm$wIPp!/u'7GT,=j42.1-*?qg_:U+k)ARFiXfSt~Cx`y^WNZb",
	"O)e8-_Ii,5YD(>xy04+t3!lLM{zH%}9AE*RS?=;Bmcu62$jW'&fdG]P/w<~b:p.JoC^QnagN\\FkX`TUK\"|r1Vhvq@Zs#7[",
	"L>Ev'g2I)DqpC3W\\5+(HiSYAj_&6r]TOk4:aJ1h;@`%^y#*9s~/!MmG[zR?|,\"-V0Un=XefQtb7oxwNl.K$Zd8}{<FBPuc",
	"[|$PAcq9,SL-2'\"sxYgQfoWNtmO*81hweM\\F~J:D;{jiy/Tu=%G<a]!dk}>vBn+`b3rRIlZ4&)50C?@V#^X6K(UpH7zE_.",
	"&Y_X(t|m,o;'E6QA1D5zkiZc@^-\"fW/8#T!RFrwK4HCa>`nx+d%]u9s.MjS0Nh:U[GeVI<L*g~J?2{)=pq3lyOP}v\\b$B7",
	"VS[4hUy2r-{t@;He3iGE]$~_P\\MDqQO/7J08m}9Zv^TNCAbl+?o>,K'1\"<Y5Wfgk(%n6Idjcwzx!&`)Ba*=RF.:#uLps|X",
	"v3<\"wMSFi(>?Gq]b)%=:8Y0L#O7$/d4Wz@~[Ec.{mADUohIg;5yT`Qa_Rkp&u6'BH9*j1^e|-ZKfX,Jl\\Nt}rnVx+CsP!2",
	"v)#r0fYax-(8ih>y!Mn3=,?I;uzF{Lo2HkBgCs[4A&/9tqGUW%\".TwJ]+QVOl\\5~6_`*@X^eNS7RKZ':EmcP<jdbp$}|1D",
	"gR@Mf91}BOUIx*-ui$\"H%`z_Jh:r/k[D4\\X#V0q+(bWEc5]?,s>|&ol=a6m.2L;7NQPvZdT)ypwC8t^nKF3G!A~Sj<'{Ye",
	"\"SF^48:<ZR6>X&TaU1,`=7\\fbtJGjemyqurzL@kABo]/Hxi!hlCW['c}{E*vN)s5wY.QMD3V;~%+P|_IK?9g-2#(0dp$On",
	"#gtp+>:v?JTbQ).xC8_W*`-j6FR@L\\%[|PSE<HNOG{za4nm^A7uqiU=wY;'5,I1s\"kM2y30dKcVB!f9$Zor(Xe}&~h/Dl]",
	"3{uO\\m2}fxd\"&VYS>:wp+6voZ]14Ueh-7@asbzK'n~#Rqjl(HcPD;F$W<Q*G[0Er?,g_BC.|9/i5TI!^JXt=8kLyM)`AN%",
	"C'wPJDAv[LgkycsWl%`j~-(9#.0|+7*VhH=O@;_X1tmnT^2B?8UYp3q&QuzEMK:N{F\"/}]doGir,<aI$b45!\\)>Sx6ReZf",
	"ukGz\"r'7M,j5c4FW)sqg-x0oaS$U*dJV(!%Ri>;\\TB?yNbXD`1QHI~8A{ml}nPE^O/w|]pLt#[<2KvC+Y@.:e9_=&hZ36f",
	"-7<maG8gu,D~&5jqO9c(s>1/T%J])LzKIp:\"B^4oQ$U'}h0;`CY23w_=?Z*eP.[iXnNVR#kydx!HStWb{E6+MfAvl|rF\\@",
	"[DZ0{Ko#9Sq.(`5E3*?m>j,b-MgBAv6}4F1flr'+x%;/)&aIXJ@YOC8|i\\t\"Pnhzc~$R_yQ!s^dU<=VLNke2pHGT7uW:]w",
	"df@53z+7iW=pED?IMJ#$;&/_m~t0<oSw'Bc(QgkUC2}N*L[vV68^P4`-YaH{hK>jX.elG9AyR\\]FZuTr):1%!|Oq,nsbx\"",
	";Hp)A*12>_QXP(+hfcEvme^`Z6@{Y7gu?r4wo|B$5nD\\bUG[TjkOW~Fz-ali}.t#:C0L]'Vd/=3xSNq9I!8\"RKs%J&,M<y",
	"r`Gh~g-]N+O%;\"Q>$5^qk{#b4(EvI=URp.9!_\\J7&y*@[MDdfVK8m1cnoW<e/PA)x,ljHXB6Si?0tTa2sZL:Yuz|'C3F}w",
	"QTwav:LZ\\)x=9e*R,W'!/f\"$bP7GFtXyjAk8%1#-ci]?H5Cn^EJ2V4M~Ss{mU_I3qB}d`u0+(@;z&[OK|p6.>loN<DgrYh",
	"U6;\\YJvXW^nw?Vc[P7<-1u|E~h'>2ZpkOyo!}`gD5/=48@0CaML{t\"qr9RsQdIj]fKT(HGN.#AS+m3bxz%F&eiB:)*,$l_",
	"S{i\"?W8K76zGnbaH!PI;%td1>TjvM\\`x(.'@~k#]E_Cqw0Xe&^+|ym)F*4QhJOB-:ZNs[Ro3gLc,2$lp}<f59YD=Vr/uAU",
	"SE,s&2uPaoL}5dY>A0/HD1mi7lg\"x\\N_hW6`RUG:'I8wJ9.<j{t*fQ]Z-(kVMz|^q$pO%!?B4K)[3X=v#cF@+yTb~nr;Ce",
	"A{3!.4G<^/k[vz\"Ka=7,h@YwVJ}dXNm]\\09xour1*_B|%>$`P5)eSbOE2y#LcMDqiQC'I&ZjlF;~6RWsHfp+-Un?t8:(Tg",
	"+?hAq~$R:UY_v}9[=;EdyVI.S5a<TDPZ4\\,g)3('X!N%Blu*G&K-^x7i/>{6rC2\"eHwpznFMokLObm8f]j`tsc@W#|JQ01",
	"8]ZrEnip7\"f3cDzF1IW6wmvh^Q(0d`quy9a-A%o:&=!k{S[VNxX$M#K@_.s}'\\U)TG>?,YC*g+<JelP5Ob2j~|/tLHBR4;",
	"^WdvLuFU{0N&;a]P[hGI}z-@O*eJHcsg=/b|`ZQ)_'%5\\Mq2$lD!<wf3XK#4Trk9>RtSnx.7Ei\"yj+86:,~B1CpVmY(o?A",
	"xPW%n{C-|$\"Oo2ar+Vh:duZDU^e#19iA!K8`,&]RE?}Mw;Jk\\b'=NzF<~>4TQ3(tXyGB0Sq[Hcs./I@)_57YLml6fjp*gv",
	"VD2{+N>w]=pU:i3jI[9Eb/YSdZy}8oeukGq_#@J6Wr\"PHsnMFKCv1f5)Bl|%&h0\\Q.a`R?<7^zx-TX*(t4!'A;m~gO$c,L",
	"~qgD\\3rc%k0BFxP76=jTH)RLVo;I4bwa,J&v\"^|n[M.CEt+`G@A_dh5-y>Q/Z!92{:e1s?S}Y<]m$pKlXfN#*UuiW8z'(O",
	"u~=8S}]gctWEX|U{2\\afm7>^Oop[';qCMz$!eZH(lTPx?\"#Fby,Kw_Rn5k0@&4LVi`QG3v9N1rh-.<6dYjI+sDBJA%)/*:",
	"/~,yg!70]Th_'E}-CndVaAsB=vuYq`\";+F9xfr$%XKc2M.L6oWz#8NGekw1P*j(4{:t)@HS?pRJbQ|[Z5il\\>DI^<O3mU&",
	"Hx/dB9o5qPR'_zKmU>,%&MYj7Q~-[lC8Z^t*vcVhN{pS+L)JO;n?F(Gy\\3]$I@1s`}=u#igE|!Ab2\"a6f<T:kD4W0ewX.r",
	"/SY*gNK9MilJWVb|kzcr7,6X?ay#3:$`%P1^Ce;p5AsH~(f+D.-!'@)nxTdh2toE0}{Qu>[G=mZ4&F]IUOwB_vLR\"qj8<\\",
	"%c(~iV;rg6aW)ZU=z.LmDjQ`<Rdv[e|\"uB32fp8N+FGqo_0\\w]$Y,>'t-sTS*I{!?^xlnEAM49J1hH}@7P#X:&OC5bKk/y",
	"bug=j@4`s:P/\\W?BY,&mx(#nk;qAV2EHC%~}6$<]TI-^hU!v8z5plr7Qt)o'XR>f|{D*+1dc[._9Z\"KSy03iwNGJaeMLFO",
	"EOP}K5&$-tJ,w_(9@SaHqGh{N.|:YMe7U~!jf3Ry=*gkX62o[Ii>BxC^bvLpdQ+zZ4nr/Wm%cAD#?\"18\\lu])<;FV0Ts`'",
	"@WY\\nm-93IpJcib/$[tvPoK<':sEFy=.frel>CN?%w+A~8*2\"^Rh{75X`qa,|_;(LTuHOB01M&VjD!QGZ)]k#dz4U}x6gS",
	"nq+E[be9MFLm3B{/@]Q*\\6~vCGK}IR\"OS&,c42dgw>khiPV'NJ:U._jf`pWaZY=DXsHx-!(z5#A?o%y^8$T0;r<ul7|)1t",
	"IB8!{^+]pe-[<`NKWS%P'M9j;Hxn1hql2G4uAyYb=R:ED#mgiO7kC,&QwL*ZXd0s5r.3\\>Joc)?FvV6aU~}@/T(f_t\"|$z",
	"(ho;.)-l[L#J>RG\"Xs{w69%ve@W/Z8T1bq7*=}uNS$'H:<D]cx4\\OC&gUBFM_VIEmazA+?k0`j~nY2pQ,dyfK5i^P3|tr!",
	"+Gm,5i<:_R6X]/>)gsky1=QdT^F.03n@I4N&!{*oCEKV-cBWS%A[rpu9;DOb\"}'v7P~xhlfw8H#j2aqLetY`U|Z$(z?M\\J",
	"ikRVUna%BWE&g*oC-D$t#~MJ?1rsXL<_j7>{8.T:=wK9\"!puq^'H|5O[4}0yxe`dfbS(,G/YNAm\\+Zz;@F2vIl)Q6Ph3]c",
	"f+B?4AUE<a>~JG#M=hkq-,|S9yt.&j}eCiPI@Od^Yv3`c[zN0!u7'5/Z6;rL$s]mKX8o{Fx(b2T_W\")DH:pRwglV\\%*1nQ",
	"}adxS\"7?Av<[sc(>91)z@/ni:VrkTRXW]HPQ#q*\\^_;.~NJMFyeGtu-L0=UKD+4Yj$OBC5I2`o%fh{8'gmZ3lbp&|,!Ew6",
	"VH4olvCi|&Uq10w-:bK)6RBJneX#kcZ5>`'h9?\\[]8%g^.d\"O7{a+*W}jmf;@G!pT$PSA3<Iy,trx=zQ2Y(u_MEDsN~FL/",
	"pSg\",!nlVa_<H^5+ZcYrJuF8q.{fhK'|w#$-U4Aj3C7\\DLeQt}m]&MP6bG*=%)BN2z[@(ykT?W0dRX~:o`sxO1i9;E/>Iv",
	":s=VI*-.$a[6j0%9QRJ2UOeMXz/C1Nu}Wg>rxAGi;B&{4~t`3Pc+<H\\wT]Zo,5_^!SE|8Fdpkh\"qb'#yLYvfm@l?nK)D7(",
	"$c,nmF+1)O|Y`2zJD\"%pkI'>rNqw[*tG:b?P=}K<;AQ7.g5RLh#9lZV]H/WB8E@TSva^Xxs3_(!{jMdCf0~-Uu4oi6y\\e&",
	"'.ycspRVE8<i=BWY,q2&k!lU1n|_Qd$FgDG(05xKLPJa*v4w~I^/Mro\\9#tjX+;{z`}%b:-C6hSH7f?]em>\")[Tu@O3ANZ",
	"TqV{$B^&(A6M:O2eRJ};n-z<H\"dKp+!cjy~7ux'\\8QIiY,=*rCaDvfP1Um>|s5Sh.?Z9l%43gEFGbtW_X/[)]N0`L#@kow",
	";YDE.d5=FS,H-Lm_4%0zA~n`I)B$vuU8>qGi&xa'Wfpyt!/Ow9#\\[1<K3cRr}:@2^Zk?lgXjs]Mb\"eC6V|T*Q7+J{PNh(o",
	"J^A]9;Be(`}W@Nl5ks[fb,'mpM=07q$K8~LY|)wZyn3G.duvRT6O-tQ/H%!D<*4\\X{&hP?EgV+zio\"jU:r_SCc>2IxFa#1",
	"`Y1*DUN@b.=BhwZ%>~k/F'G<}&T;:i+g7QOKvtcj$MIVs5\\Ax!9,q_\"|H2f4m8W{d](Rn#0P)uS3eCLXy6Jz?o^pra-l[E",
	"hPs4qpuQE%CN'M:zyZc/.U8^]=LS?{j916d*-lrF_`}$#)75(\"&x@<J~3KIwvHgmnib+D,|oWBfGO0[k\\AY!V2Rt;XaeT>"
};
/******************************************************************************************/
/*****************************************局部函数声明*************************************/

/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

/***************************************************************************************************
*FunctionName：MyDencrypt
*Description：二维码数据密文转明文
*Input：source -- 密文数据
*		target -- 转换后的明文
*		len -- 密文和明文的长度（密文和明文长度一样）
*Output：None
*Author：xsx
*Data：2016年4月25日15:32:39
***************************************************************************************************/
MyState_TypeDef MyDencrypt(char *source, char *target, unsigned short len)
{
	PointBuffer * TempPointBuffer = NULL;
	MyState_TypeDef statues = My_Fail;
	
	TempPointBuffer = MyMalloc(PointBufferStructSize);
	if(TempPointBuffer)
	{
		memset(TempPointBuffer, 0, PointBufferStructSize);
		
		for(TempPointBuffer->i=0; TempPointBuffer->i<len ;TempPointBuffer->i++)
		{
			TempPointBuffer->tempIndex = (TempPointBuffer->i + len) % 54;
			TempPointBuffer->tempV1 = myKey[TempPointBuffer->tempIndex] - 33;
			TempPointBuffer->tempV2 = source[TempPointBuffer->i];
			
			for(TempPointBuffer->j=0; TempPointBuffer->j<94; TempPointBuffer->j++)
			{
				if(myWord[TempPointBuffer->tempV1][TempPointBuffer->j] == TempPointBuffer->tempV2)
				{
					*target++ = TempPointBuffer->j+33;
					break;	
				}
			}
		}
		
		statues = My_Pass;
	}
	
	MyFree(TempPointBuffer);
		
	return statues;
}

