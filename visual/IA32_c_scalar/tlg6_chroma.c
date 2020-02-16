/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include <mmintrin.h>
#include <xmmintrin.h>
#include "tvpgl_ia32_intf_c.h"

static tjs_uint64 g_mask      = 0x0000ff000000ff00ull;
static tjs_uint64 b_mask      = 0x000000ff000000ffull;
static tjs_uint64 r_mask      = 0x00ff000000ff0000ull;
static tjs_uint64 a_mask      = 0xff000000ff000000ull;
static tjs_uint64 g_d_mask    = 0x0000fe000000fe00ull;
static tjs_uint64 r_d_mask    = 0x00fe000000fe0000ull;
static tjs_uint64 b_d_mask    = 0x000000fe000000feull;
static tjs_uint64 avg_mask_fe = 0xfefefefefefefefeull;
static tjs_uint64 avg_mask_01 = 0x0101010101010101ull;

void __cdecl TVPTLG6DecodeLine_mmx_c(tjs_uint32 *prevline, tjs_uint32 *curline, tjs_int width, tjs_int block_count, tjs_uint8 *filtertypes, tjs_int skipblockbytes, tjs_uint32 *input, tjs_uint32 initialp, tjs_int oddskip, tjs_int dir)
{
	int          v10;             // edx
	tjs_uint32 * v11;             // edi
	tjs_uint32 * v12;             // esi
	__m64        v13;             // mm0
	__m64        v14;             // mm6
	tjs_uint32 * v15;             // eax
	int          v16;             // ecx
	signed int   v17;             // ebx
	__m64        v18;             // mm4
	__m64        v19;             // mm2
	__m64        v20;             // mm6
	__m64        v21;             // mm3
	__m64        v22;             // mm1
	__m64        v23;             // mm7
	__m64        v24;             // mm2
	__m64        v25;             // mm3
	__m64        v26;             // mm1
	__m64        v27;             // mm4
	__m64        v28;             // mm2
	__m64        v29;             // mm6
	__m64        v30;             // mm3
	__m64        v31;             // mm1
	__m64        v32;             // mm7
	__m64        v33;             // mm2
	__m64        v34;             // mm3
	__m64        v35;             // mm1
	__m64        v36;             // mm4
	__m64        v37;             // mm6
	__m64        v38;             // mm7
	__m64        v39;             // mm4
	__m64        v40;             // mm6
	__m64        v41;             // mm7
	__m64        v42;             // mm4
	__m64        v43;             // mm2
	__m64        v44;             // mm6
	__m64        v45;             // mm3
	__m64        v46;             // mm1
	__m64        v47;             // mm7
	__m64        v48;             // mm2
	__m64        v49;             // mm3
	__m64        v50;             // mm1
	__m64        v51;             // mm4
	__m64        v52;             // mm2
	__m64        v53;             // mm6
	__m64        v54;             // mm3
	__m64        v55;             // mm1
	__m64        v56;             // mm7
	__m64        v57;             // mm2
	__m64        v58;             // mm3
	__m64        v59;             // mm1
	__m64        v60;             // mm4
	__m64        v61;             // mm6
	__m64        v62;             // mm7
	__m64        v63;             // mm4
	__m64        v64;             // mm6
	__m64        v65;             // mm7
	__m64        v66;             // mm4
	__m64        v67;             // mm4
	__m64        v68;             // mm2
	__m64        v69;             // mm6
	__m64        v70;             // mm3
	__m64        v71;             // mm1
	__m64        v72;             // mm7
	__m64        v73;             // mm2
	__m64        v74;             // mm3
	__m64        v75;             // mm1
	__m64        v76;             // mm4
	__m64        v77;             // mm4
	__m64        v78;             // mm2
	__m64        v79;             // mm6
	__m64        v80;             // mm3
	__m64        v81;             // mm1
	__m64        v82;             // mm7
	__m64        v83;             // mm2
	__m64        v84;             // mm3
	__m64        v85;             // mm1
	__m64        v86;             // mm4
	__m64        v87;             // mm4
	__m64        v88;             // mm6
	__m64        v89;             // mm7
	__m64        v90;             // mm4
	__m64        v91;             // mm4
	__m64        v92;             // mm6
	__m64        v93;             // mm7
	__m64        v94;             // mm4
	__m64        v95;             // mm4
	__m64        v96;             // mm2
	__m64        v97;             // mm6
	__m64        v98;             // mm3
	__m64        v99;             // mm1
	__m64        v100;            // mm7
	__m64        v101;            // mm2
	__m64        v102;            // mm3
	__m64        v103;            // mm1
	__m64        v104;            // mm4
	__m64        v105;            // mm4
	__m64        v106;            // mm2
	__m64        v107;            // mm6
	__m64        v108;            // mm3
	__m64        v109;            // mm1
	__m64        v110;            // mm7
	__m64        v111;            // mm2
	__m64        v112;            // mm3
	__m64        v113;            // mm1
	__m64        v114;            // mm4
	__m64        v115;            // mm4
	__m64        v116;            // mm6
	__m64        v117;            // mm7
	__m64        v118;            // mm4
	__m64        v119;            // mm4
	__m64        v120;            // mm6
	__m64        v121;            // mm7
	__m64        v122;            // mm4
	__m64        v123;            // mm4
	__m64        v124;            // mm4
	__m64        v125;            // mm2
	__m64        v126;            // mm6
	__m64        v127;            // mm3
	__m64        v128;            // mm1
	__m64        v129;            // mm7
	__m64        v130;            // mm2
	__m64        v131;            // mm3
	__m64        v132;            // mm1
	__m64        v133;            // mm4
	__m64        v134;            // mm4
	__m64        v135;            // mm4
	__m64        v136;            // mm2
	__m64        v137;            // mm6
	__m64        v138;            // mm3
	__m64        v139;            // mm1
	__m64        v140;            // mm7
	__m64        v141;            // mm2
	__m64        v142;            // mm3
	__m64        v143;            // mm1
	__m64        v144;            // mm4
	__m64        v145;            // mm4
	__m64        v146;            // mm4
	__m64        v147;            // mm6
	__m64        v148;            // mm7
	__m64        v149;            // mm4
	__m64        v150;            // mm4
	__m64        v151;            // mm4
	__m64        v152;            // mm6
	__m64        v153;            // mm7
	__m64        v154;            // mm4
	__m64        v155;            // mm4
	__m64        v156;            // mm2
	__m64        v157;            // mm6
	__m64        v158;            // mm3
	__m64        v159;            // mm1
	__m64        v160;            // mm7
	__m64        v161;            // mm2
	__m64        v162;            // mm3
	__m64        v163;            // mm1
	__m64        v164;            // mm4
	__m64        v165;            // mm4
	__m64        v166;            // mm2
	__m64        v167;            // mm6
	__m64        v168;            // mm3
	__m64        v169;            // mm1
	__m64        v170;            // mm7
	__m64        v171;            // mm2
	__m64        v172;            // mm3
	__m64        v173;            // mm1
	__m64        v174;            // mm4
	__m64        v175;            // mm4
	__m64        v176;            // mm6
	__m64        v177;            // mm7
	__m64        v178;            // mm4
	__m64        v179;            // mm4
	__m64        v180;            // mm6
	__m64        v181;            // mm7
	__m64        v182;            // mm4
	__m64        v183;            // mm2
	__m64        v184;            // mm6
	__m64        v185;            // mm3
	__m64        v186;            // mm1
	__m64        v187;            // mm7
	__m64        v188;            // mm2
	__m64        v189;            // mm3
	__m64        v190;            // mm1
	__m64        v191;            // mm4
	__m64        v192;            // mm2
	__m64        v193;            // mm6
	__m64        v194;            // mm3
	__m64        v195;            // mm1
	__m64        v196;            // mm7
	__m64        v197;            // mm2
	__m64        v198;            // mm3
	__m64        v199;            // mm1
	__m64        v200;            // mm4
	__m64        v201;            // mm6
	__m64        v202;            // mm7
	__m64        v203;            // mm4
	__m64        v204;            // mm6
	__m64        v205;            // mm7
	__m64        v206;            // mm4
	__m64        v207;            // mm2
	__m64        v208;            // mm6
	__m64        v209;            // mm3
	__m64        v210;            // mm1
	__m64        v211;            // mm7
	__m64        v212;            // mm2
	__m64        v213;            // mm3
	__m64        v214;            // mm1
	__m64        v215;            // mm4
	__m64        v216;            // mm2
	__m64        v217;            // mm6
	__m64        v218;            // mm3
	__m64        v219;            // mm1
	__m64        v220;            // mm7
	__m64        v221;            // mm2
	__m64        v222;            // mm3
	__m64        v223;            // mm1
	__m64        v224;            // mm4
	__m64        v225;            // mm6
	__m64        v226;            // mm7
	__m64        v227;            // mm4
	__m64        v228;            // mm6
	__m64        v229;            // mm7
	__m64        v230;            // mm4
	__m64        v231;            // mm2
	__m64        v232;            // mm6
	__m64        v233;            // mm3
	__m64        v234;            // mm1
	__m64        v235;            // mm7
	__m64        v236;            // mm2
	__m64        v237;            // mm3
	__m64        v238;            // mm1
	__m64        v239;            // mm4
	__m64        v240;            // mm2
	__m64        v241;            // mm6
	__m64        v242;            // mm3
	__m64        v243;            // mm1
	__m64        v244;            // mm7
	__m64        v245;            // mm2
	__m64        v246;            // mm3
	__m64        v247;            // mm1
	__m64        v248;            // mm4
	__m64        v249;            // mm6
	__m64        v250;            // mm7
	__m64        v251;            // mm4
	__m64        v252;            // mm6
	__m64        v253;            // mm7
	__m64        v254;            // mm4
	__m64        v255;            // mm4
	__m64        v256;            // mm4
	__m64        v257;            // mm2
	__m64        v258;            // mm6
	__m64        v259;            // mm3
	__m64        v260;            // mm1
	__m64        v261;            // mm7
	__m64        v262;            // mm2
	__m64        v263;            // mm3
	__m64        v264;            // mm1
	__m64        v265;            // mm4
	__m64        v266;            // mm4
	__m64        v267;            // mm4
	__m64        v268;            // mm2
	__m64        v269;            // mm6
	__m64        v270;            // mm3
	__m64        v271;            // mm1
	__m64        v272;            // mm7
	__m64        v273;            // mm2
	__m64        v274;            // mm3
	__m64        v275;            // mm1
	__m64        v276;            // mm4
	__m64        v277;            // mm4
	__m64        v278;            // mm4
	__m64        v279;            // mm6
	__m64        v280;            // mm7
	__m64        v281;            // mm4
	__m64        v282;            // mm4
	__m64        v283;            // mm4
	__m64        v284;            // mm6
	__m64        v285;            // mm7
	__m64        v286;            // mm4
	__m64        v287;            // mm2
	__m64        v288;            // mm6
	__m64        v289;            // mm3
	__m64        v290;            // mm1
	__m64        v291;            // mm7
	__m64        v292;            // mm2
	__m64        v293;            // mm3
	__m64        v294;            // mm1
	__m64        v295;            // mm4
	__m64        v296;            // mm2
	__m64        v297;            // mm6
	__m64        v298;            // mm3
	__m64        v299;            // mm1
	__m64        v300;            // mm7
	__m64        v301;            // mm2
	__m64        v302;            // mm3
	__m64        v303;            // mm1
	__m64        v304;            // mm4
	__m64        v305;            // mm6
	__m64        v306;            // mm7
	__m64        v307;            // mm4
	__m64        v308;            // mm6
	__m64        v309;            // mm7
	__m64        v310;            // mm4
	__m64        v311;            // mm2
	__m64        v312;            // mm6
	__m64        v313;            // mm3
	__m64        v314;            // mm1
	__m64        v315;            // mm7
	__m64        v316;            // mm2
	__m64        v317;            // mm3
	__m64        v318;            // mm1
	__m64        v319;            // mm4
	__m64        v320;            // mm2
	__m64        v321;            // mm6
	__m64        v322;            // mm3
	__m64        v323;            // mm1
	__m64        v324;            // mm7
	__m64        v325;            // mm2
	__m64        v326;            // mm3
	__m64        v327;            // mm1
	__m64        v328;            // mm4
	__m64        v329;            // mm6
	__m64        v330;            // mm7
	__m64        v331;            // mm4
	__m64        v332;            // mm6
	__m64        v333;            // mm7
	__m64        v348;            // mm4
	__m64        v349;            // mm4
	__m64        v350;            // mm2
	__m64        v351;            // mm6
	__m64        v352;            // mm3
	__m64        v353;            // mm1
	__m64        v354;            // mm7
	__m64        v355;            // mm2
	__m64        v356;            // mm3
	__m64        v357;            // mm1
	__m64        v366;            // mm4
	__m64        v367;            // mm4
	__m64        v368;            // mm6
	__m64        v369;            // mm7
	__m64        v385;            // mm4
	__m64        v386;            // mm4
	__m64        v387;            // mm4
	__m64        v388;            // mm2
	__m64        v389;            // mm6
	__m64        v390;            // mm3
	__m64        v391;            // mm1
	__m64        v392;            // mm7
	__m64        v393;            // mm2
	__m64        v394;            // mm3
	__m64        v395;            // mm1
	__m64        v405;            // mm4
	__m64        v406;            // mm4
	__m64        v407;            // mm4
	__m64        v408;            // mm6
	__m64        v409;            // mm7
	__m64        v425;            // mm4
	__m64        v426;            // mm4
	__m64        v427;            // mm4
	__m64        v428;            // mm2
	__m64        v429;            // mm6
	__m64        v430;            // mm3
	__m64        v431;            // mm1
	__m64        v432;            // mm7
	__m64        v433;            // mm2
	__m64        v434;            // mm3
	__m64        v435;            // mm1
	__m64        v445;            // mm4
	__m64        v446;            // mm4
	__m64        v447;            // mm4
	__m64        v448;            // mm6
	__m64        v449;            // mm7
	__m64        v463;            // mm4
	__m64        v464;            // mm2
	__m64        v465;            // mm6
	__m64        v466;            // mm3
	__m64        v467;            // mm1
	__m64        v468;            // mm7
	__m64        v469;            // mm2
	__m64        v470;            // mm3
	__m64        v471;            // mm1
	__m64        v479;            // mm4
	__m64        v480;            // mm6
	__m64        v481;            // mm7
	char         v482;            // [esp+0h] [ebp-Ch]
	signed int   v483;            // [esp+4h] [ebp-8h]
	tjs_int      skipblockbytesa; // [esp+3Ch] [ebp+30h]
	unsigned int oddskipa;        // [esp+48h] [ebp+3Ch]

	if (dir & 1)
	{
		v483            = 8;
		skipblockbytesa = 4 * skipblockbytes - 32;
	}
	else
	{
		v483            = -8;
		skipblockbytesa = 4 * skipblockbytes + 32;
		input += 7;
	}
	oddskipa = 32 * oddskip;
	v10      = 0;
	v482     = 0;
	v11      = curline;
	v12      = prevline;
	v13      = _mm_cvtsi32_si64(initialp);
	v14      = v13;
	v15      = &input[oddskipa / 4];
	while (2)
	{
		v16 = *filtertypes;
		v17 = 4;
		v482 ^= 1u;
		if (v482)
			v15 = (tjs_uint32 *)((char *)v15 - oddskipa);
		else
			v15 = (tjs_uint32 *)((char *)v15 + oddskipa);
		++filtertypes;
		if (dir & 1)
		{
			switch (v16)
			{
				case 0:
					do
					{
						v18 = *(__m64 *)((char *)v15 + v10);
						v19 = v14;
						v20 = _mm_cvtsi32_si64(*v12);
						v21 = _m_paddb(_m_psubusb(v13, v20), v20);
						v22 = _m_psubb(v20, _m_psubusb(v20, v13));
						v23 = _m_psubb(
							_m_paddb(_m_paddb(*(__m64 *)((char *)v15 + v10), v21), v22),
							_m_paddb(_m_psubusb(_m_psubb(v21, _m_psubusb(v21, v19)), v22), v22));
						*v11 = _mm_cvtsi64_si32(v23);
						v10 += v483;
						v24 = v20;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v25 = _m_paddb(_m_psubusb(v23, v14), v14);
						v26 = _m_psubb(v14, _m_psubusb(v14, v23));
						v13 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v18, 0x20u), v25), v26),
							_m_paddb(_m_psubusb(_m_psubb(v25, _m_psubusb(v25, v24)), v26), v26));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 1:
					do
					{
						v36 = *(__m64 *)((char *)v15 + v10);
						v37 = _mm_cvtsi32_si64(*v12);
						v38 = _m_paddb(
							_m_paddb(*(__m64 *)((char *)v15 + v10), _m_pand(_m_por(v13, v37), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v37, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v38);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(_m_psrlqi(v36, 0x20u), _m_pand(_m_por(v38, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v38, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 2:
					do
					{
						v42 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10),
								_m_psllqi(_m_pand(*(__m64 *)((char *)v15 + v10), (__m64)g_mask), 8u)),
							_m_psrlqi(_m_pand(*(__m64 *)((char *)v15 + v10), (__m64)g_mask), 8u));
						v43 = v14;
						v44 = _mm_cvtsi32_si64(*v12);
						v45 = _m_paddb(_m_psubusb(v13, v44), v44);
						v46 = _m_psubb(v44, _m_psubusb(v44, v13));
						v47 = _m_psubb(
							_m_paddb(_m_paddb(v42, v45), v46),
							_m_paddb(_m_psubusb(_m_psubb(v45, _m_psubusb(v45, v43)), v46), v46));
						*v11 = _mm_cvtsi64_si32(v47);
						v10 += v483;
						v48 = v44;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v49 = _m_paddb(_m_psubusb(v47, v14), v14);
						v50 = _m_psubb(v14, _m_psubusb(v14, v47));
						v13 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v42, 0x20u), v49), v50),
							_m_paddb(_m_psubusb(_m_psubb(v49, _m_psubusb(v49, v48)), v50), v50));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 3:
					do
					{
						v60 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10),
								_m_psllqi(_m_pand(*(__m64 *)((char *)v15 + v10), (__m64)g_mask), 8u)),
							_m_psrlqi(_m_pand(*(__m64 *)((char *)v15 + v10), (__m64)g_mask), 8u));
						v61 = _mm_cvtsi32_si64(*v12);
						v62 = _m_paddb(
							_m_paddb(v60, _m_pand(_m_por(v13, v61), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v61, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v62);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(_m_psrlqi(v60, 0x20u), _m_pand(_m_por(v62, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v62, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 4:
					do
					{
						v66 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask));
						v67 = _m_paddb(v66, _m_pand(_m_psllqi(v66, 8u), (__m64)r_mask));
						v68 = v14;
						v69 = _mm_cvtsi32_si64(*v12);
						v70 = _m_paddb(_m_psubusb(v13, v69), v69);
						v71 = _m_psubb(v69, _m_psubusb(v69, v13));
						v72 = _m_psubb(
							_m_paddb(_m_paddb(v67, v70), v71),
							_m_paddb(_m_psubusb(_m_psubb(v70, _m_psubusb(v70, v68)), v71), v71));
						*v11 = _mm_cvtsi64_si32(v72);
						v10 += v483;
						v73 = v69;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v74 = _m_paddb(_m_psubusb(v72, v14), v14);
						v75 = _m_psubb(v14, _m_psubusb(v14, v72));
						v13 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v67, 0x20u), v74), v75),
							_m_paddb(_m_psubusb(_m_psubb(v74, _m_psubusb(v74, v73)), v75), v75));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 5:
					do
					{
						v86 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask));
						v87 = _m_paddb(v86, _m_pand(_m_psllqi(v86, 8u), (__m64)r_mask));
						v88 = _mm_cvtsi32_si64(*v12);
						v89 = _m_paddb(
							_m_paddb(v87, _m_pand(_m_por(v13, v88), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v88, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v89);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(_m_psrlqi(v87, 0x20u), _m_pand(_m_por(v89, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v89, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 6:
					do
					{
						v94 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask));
						v95  = _m_paddb(v94, _m_pand(_m_psrlqi(v94, 8u), (__m64)b_mask));
						v96  = v14;
						v97  = _mm_cvtsi32_si64(*v12);
						v98  = _m_paddb(_m_psubusb(v13, v97), v97);
						v99  = _m_psubb(v97, _m_psubusb(v97, v13));
						v100 = _m_psubb(
							_m_paddb(_m_paddb(v95, v98), v99),
							_m_paddb(_m_psubusb(_m_psubb(v98, _m_psubusb(v98, v96)), v99), v99));
						*v11 = _mm_cvtsi64_si32(v100);
						v10 += v483;
						v101 = v97;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v102 = _m_paddb(_m_psubusb(v100, v14), v14);
						v103 = _m_psubb(v14, _m_psubusb(v14, v100));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v95, 0x20u), v102), v103),
                            _m_paddb(_m_psubusb(_m_psubb(v102, _m_psubusb(v102, v101)), v103), v103));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 7:
					do
					{
						v114 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask));
						v115 = _m_paddb(v114, _m_pand(_m_psrlqi(v114, 8u), (__m64)b_mask));
						v116 = _mm_cvtsi32_si64(*v12);
						v117 = _m_paddb(
							_m_paddb(v115, _m_pand(_m_por(v13, v116), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v116, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v117);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(_m_psrlqi(v115, 0x20u), _m_pand(_m_por(v117, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v117, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 8:
					do
					{
						v122 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)b_mask));
						v123 = _m_paddb(v122, _m_pand(_m_psllqi(v122, 8u), (__m64)g_mask));
						v124 = _m_paddb(v123, _m_pand(_m_psllqi(v123, 8u), (__m64)r_mask));
						v125 = v14;
						v126 = _mm_cvtsi32_si64(*v12);
						v127 = _m_paddb(_m_psubusb(v13, v126), v126);
						v128 = _m_psubb(v126, _m_psubusb(v126, v13));
						v129 = _m_psubb(
							_m_paddb(_m_paddb(v124, v127), v128),
							_m_paddb(_m_psubusb(_m_psubb(v127, _m_psubusb(v127, v125)), v128), v128));
						*v11 = _mm_cvtsi64_si32(v129);
						v10 += v483;
						v130 = v126;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v131 = _m_paddb(_m_psubusb(v129, v14), v14);
						v132 = _m_psubb(v14, _m_psubusb(v14, v129));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v124, 0x20u), v131), v132),
                            _m_paddb(_m_psubusb(_m_psubb(v131, _m_psubusb(v131, v130)), v132), v132));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 9:
					do
					{
						v144 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)b_mask));
						v145 = _m_paddb(v144, _m_pand(_m_psllqi(v144, 8u), (__m64)g_mask));
						v146 = _m_paddb(v145, _m_pand(_m_psllqi(v145, 8u), (__m64)r_mask));
						v147 = _mm_cvtsi32_si64(*v12);
						v148 = _m_paddb(
							_m_paddb(v146, _m_pand(_m_por(v13, v147), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v147, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v148);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(_m_psrlqi(v146, 0x20u), _m_pand(_m_por(v148, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v148, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 10:
					do
					{
						v154 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)b_mask));
						v155 = _m_paddb(v154, _m_pand(_m_psllqi(v154, 8u), (__m64)g_mask));
						v156 = v14;
						v157 = _mm_cvtsi32_si64(*v12);
						v158 = _m_paddb(_m_psubusb(v13, v157), v157);
						v159 = _m_psubb(v157, _m_psubusb(v157, v13));
						v160 = _m_psubb(
							_m_paddb(_m_paddb(v155, v158), v159),
							_m_paddb(_m_psubusb(_m_psubb(v158, _m_psubusb(v158, v156)), v159), v159));
						*v11 = _mm_cvtsi64_si32(v160);
						v10 += v483;
						v161 = v157;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v162 = _m_paddb(_m_psubusb(v160, v14), v14);
						v163 = _m_psubb(v14, _m_psubusb(v14, v160));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v155, 0x20u), v162), v163),
                            _m_paddb(_m_psubusb(_m_psubb(v162, _m_psubusb(v162, v161)), v163), v163));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 11:
					do
					{
						v174 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)b_mask));
						v175 = _m_paddb(v174, _m_pand(_m_psllqi(v174, 8u), (__m64)g_mask));
						v176 = _mm_cvtsi32_si64(*v12);
						v177 = _m_paddb(
							_m_paddb(v175, _m_pand(_m_por(v13, v176), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v176, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v177);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(_m_psrlqi(v175, 0x20u), _m_pand(_m_por(v177, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v177, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 12:
					do
					{
						v182 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)b_mask));
						v183 = v14;
						v184 = _mm_cvtsi32_si64(*v12);
						v185 = _m_paddb(_m_psubusb(v13, v184), v184);
						v186 = _m_psubb(v184, _m_psubusb(v184, v13));
						v187 = _m_psubb(
							_m_paddb(_m_paddb(v182, v185), v186),
							_m_paddb(_m_psubusb(_m_psubb(v185, _m_psubusb(v185, v183)), v186), v186));
						*v11 = _mm_cvtsi64_si32(v187);
						v10 += v483;
						v188 = v184;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v189 = _m_paddb(_m_psubusb(v187, v14), v14);
						v190 = _m_psubb(v14, _m_psubusb(v14, v187));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v182, 0x20u), v189), v190),
                            _m_paddb(_m_psubusb(_m_psubb(v189, _m_psubusb(v189, v188)), v190), v190));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 13:
					do
					{
						v200 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)b_mask));
						v201 = _mm_cvtsi32_si64(*v12);
						v202 = _m_paddb(
							_m_paddb(v200, _m_pand(_m_por(v13, v201), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v201, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v202);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(_m_psrlqi(v200, 0x20u), _m_pand(_m_por(v202, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v202, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 14:
					do
					{
						v206 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask));
						v207 = v14;
						v208 = _mm_cvtsi32_si64(*v12);
						v209 = _m_paddb(_m_psubusb(v13, v208), v208);
						v210 = _m_psubb(v208, _m_psubusb(v208, v13));
						v211 = _m_psubb(
							_m_paddb(_m_paddb(v206, v209), v210),
							_m_paddb(_m_psubusb(_m_psubb(v209, _m_psubusb(v209, v207)), v210), v210));
						*v11 = _mm_cvtsi64_si32(v211);
						v10 += v483;
						v212 = v208;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v213 = _m_paddb(_m_psubusb(v211, v14), v14);
						v214 = _m_psubb(v14, _m_psubusb(v14, v211));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v206, 0x20u), v213), v214),
                            _m_paddb(_m_psubusb(_m_psubb(v213, _m_psubusb(v213, v212)), v214), v214));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 15:
					do
					{
						v224 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask));
						v225 = _mm_cvtsi32_si64(*v12);
						v226 = _m_paddb(
							_m_paddb(v224, _m_pand(_m_por(v13, v225), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v225, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v226);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(_m_psrlqi(v224, 0x20u), _m_pand(_m_por(v226, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v226, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 16:
					do
					{
						v230 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)r_mask));
						v231 = v14;
						v232 = _mm_cvtsi32_si64(*v12);
						v233 = _m_paddb(_m_psubusb(v13, v232), v232);
						v234 = _m_psubb(v232, _m_psubusb(v232, v13));
						v235 = _m_psubb(
							_m_paddb(_m_paddb(v230, v233), v234),
							_m_paddb(_m_psubusb(_m_psubb(v233, _m_psubusb(v233, v231)), v234), v234));
						*v11 = _mm_cvtsi64_si32(v235);
						v10 += v483;
						v236 = v232;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v237 = _m_paddb(_m_psubusb(v235, v14), v14);
						v238 = _m_psubb(v14, _m_psubusb(v14, v235));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v230, 0x20u), v237), v238),
                            _m_paddb(_m_psubusb(_m_psubb(v237, _m_psubusb(v237, v236)), v238), v238));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 17:
					do
					{
						v248 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)r_mask));
						v249 = _mm_cvtsi32_si64(*v12);
						v250 = _m_paddb(
							_m_paddb(v248, _m_pand(_m_por(v13, v249), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v249, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v250);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(_m_psrlqi(v248, 0x20u), _m_pand(_m_por(v250, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v250, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 18:
					do
					{
						v254 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)r_mask));
						v255 = _m_paddb(v254, _m_pand(_m_psrlqi(v254, 8u), (__m64)g_mask));
						v256 = _m_paddb(v255, _m_pand(_m_psrlqi(v255, 8u), (__m64)b_mask));
						v257 = v14;
						v258 = _mm_cvtsi32_si64(*v12);
						v259 = _m_paddb(_m_psubusb(v13, v258), v258);
						v260 = _m_psubb(v258, _m_psubusb(v258, v13));
						v261 = _m_psubb(
							_m_paddb(_m_paddb(v256, v259), v260),
							_m_paddb(_m_psubusb(_m_psubb(v259, _m_psubusb(v259, v257)), v260), v260));
						*v11 = _mm_cvtsi64_si32(v261);
						v10 += v483;
						v262 = v258;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v263 = _m_paddb(_m_psubusb(v261, v14), v14);
						v264 = _m_psubb(v14, _m_psubusb(v14, v261));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v256, 0x20u), v263), v264),
                            _m_paddb(_m_psubusb(_m_psubb(v263, _m_psubusb(v263, v262)), v264), v264));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 19:
					do
					{
						v276 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)r_mask));
						v277 = _m_paddb(v276, _m_pand(_m_psrlqi(v276, 8u), (__m64)g_mask));
						v278 = _m_paddb(v277, _m_pand(_m_psrlqi(v277, 8u), (__m64)b_mask));
						v279 = _mm_cvtsi32_si64(*v12);
						v280 = _m_paddb(
							_m_paddb(v278, _m_pand(_m_por(v13, v279), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v279, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v280);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(_m_psrlqi(v278, 0x20u), _m_pand(_m_por(v280, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v280, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 20:
					do
					{
						v286 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10),
								_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask)),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)b_mask));
						v287 = v14;
						v288 = _mm_cvtsi32_si64(*v12);
						v289 = _m_paddb(_m_psubusb(v13, v288), v288);
						v290 = _m_psubb(v288, _m_psubusb(v288, v13));
						v291 = _m_psubb(
							_m_paddb(_m_paddb(v286, v289), v290),
							_m_paddb(_m_psubusb(_m_psubb(v289, _m_psubusb(v289, v287)), v290), v290));
						*v11 = _mm_cvtsi64_si32(v291);
						v10 += v483;
						v292 = v288;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v293 = _m_paddb(_m_psubusb(v291, v14), v14);
						v294 = _m_psubb(v14, _m_psubusb(v14, v291));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v286, 0x20u), v293), v294),
                            _m_paddb(_m_psubusb(_m_psubb(v293, _m_psubusb(v293, v292)), v294), v294));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 21:
					do
					{
						v304 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10),
								_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask)),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)b_mask));
						v305 = _mm_cvtsi32_si64(*v12);
						v306 = _m_paddb(
							_m_paddb(v304, _m_pand(_m_por(v13, v305), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v305, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v306);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(_m_psrlqi(v304, 0x20u), _m_pand(_m_por(v306, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v306, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 22:
					do
					{
						v310 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10),
								_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask)),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)r_mask));
						v311 = v14;
						v312 = _mm_cvtsi32_si64(*v12);
						v313 = _m_paddb(_m_psubusb(v13, v312), v312);
						v314 = _m_psubb(v312, _m_psubusb(v312, v13));
						v315 = _m_psubb(
							_m_paddb(_m_paddb(v310, v313), v314),
							_m_paddb(_m_psubusb(_m_psubb(v313, _m_psubusb(v313, v311)), v314), v314));
						*v11 = _mm_cvtsi64_si32(v315);
						v10 += v483;
						v316 = v312;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v317 = _m_paddb(_m_psubusb(v315, v14), v14);
						v318 = _m_psubb(v14, _m_psubusb(v14, v315));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v310, 0x20u), v317), v318),
                            _m_paddb(_m_psubusb(_m_psubb(v317, _m_psubusb(v317, v316)), v318), v318));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 23:
					do
					{
						v328 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10),
								_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask)),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)r_mask));
						v329 = _mm_cvtsi32_si64(*v12);
						v330 = _m_paddb(
							_m_paddb(v328, _m_pand(_m_por(v13, v329), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v329, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v330);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(_m_psrlqi(v328, 0x20u), _m_pand(_m_por(v330, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v330, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
			}
		}
		else
		{
			switch (v16)
			{
				case 0:
					do
					{
						v27 = *(__m64 *)((char *)v15 + v10 - 4);
						v28 = v14;
						v29 = _mm_cvtsi32_si64(*v12);
						v30 = _m_paddb(_m_psubusb(v13, v29), v29);
						v31 = _m_psubb(v29, _m_psubusb(v29, v13));
						v32 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 0x20u), v30), v31),
							_m_paddb(_m_psubusb(_m_psubb(v30, _m_psubusb(v30, v28)), v31), v31));
						*v11 = _mm_cvtsi64_si32(v32);
						v10 += v483;
						v33 = v29;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v34 = _m_paddb(_m_psubusb(v32, v14), v14);
						v35 = _m_psubb(v14, _m_psubusb(v14, v32));
						v13 = _m_psubb(
							_m_paddb(_m_paddb(v27, v34), v35),
							_m_paddb(_m_psubusb(_m_psubb(v34, _m_psubusb(v34, v33)), v35), v35));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 1:
					do
					{
						v39 = *(__m64 *)((char *)v15 + v10 - 4);
						v40 = _mm_cvtsi32_si64(*v12);
						v41 = _m_paddb(
							_m_paddb(
								_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 0x20u),
								_m_pand(_m_por(v13, v40), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v40, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v41);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(v39, _m_pand(_m_por(v41, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v41, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 2:
					do
					{
						v51 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10 - 4),
								_m_psllqi(_m_pand(*(__m64 *)((char *)v15 + v10 - 4), (__m64)g_mask), 8u)),
							_m_psrlqi(_m_pand(*(__m64 *)((char *)v15 + v10 - 4), (__m64)g_mask), 8u));
						v52 = v14;
						v53 = _mm_cvtsi32_si64(*v12);
						v54 = _m_paddb(_m_psubusb(v13, v53), v53);
						v55 = _m_psubb(v53, _m_psubusb(v53, v13));
						v56 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v51, 0x20u), v54), v55),
							_m_paddb(_m_psubusb(_m_psubb(v54, _m_psubusb(v54, v52)), v55), v55));
						*v11 = _mm_cvtsi64_si32(v56);
						v10 += v483;
						v57 = v53;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v58 = _m_paddb(_m_psubusb(v56, v14), v14);
						v59 = _m_psubb(v14, _m_psubusb(v14, v56));
						v13 = _m_psubb(
							_m_paddb(_m_paddb(v51, v58), v59),
							_m_paddb(_m_psubusb(_m_psubb(v58, _m_psubusb(v58, v57)), v59), v59));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 3:
					do
					{
						v63 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10 - 4),
								_m_psllqi(_m_pand(*(__m64 *)((char *)v15 + v10 - 4), (__m64)g_mask), 8u)),
							_m_psrlqi(_m_pand(*(__m64 *)((char *)v15 + v10 - 4), (__m64)g_mask), 8u));
						v64 = _mm_cvtsi32_si64(*v12);
						v65 = _m_paddb(
							_m_paddb(_m_psrlqi(v63, 0x20u), _m_pand(_m_por(v13, v64), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v64, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v65);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(v63, _m_pand(_m_por(v65, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v65, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 4:
					do
					{
						v76 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask));
						v77 = _m_paddb(v76, _m_pand(_m_psllqi(v76, 8u), (__m64)r_mask));
						v78 = v14;
						v79 = _mm_cvtsi32_si64(*v12);
						v80 = _m_paddb(_m_psubusb(v13, v79), v79);
						v81 = _m_psubb(v79, _m_psubusb(v79, v13));
						v82 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v77, 0x20u), v80), v81),
							_m_paddb(_m_psubusb(_m_psubb(v80, _m_psubusb(v80, v78)), v81), v81));
						*v11 = _mm_cvtsi64_si32(v82);
						v10 += v483;
						v83 = v79;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v84 = _m_paddb(_m_psubusb(v82, v14), v14);
						v85 = _m_psubb(v14, _m_psubusb(v14, v82));
						v13 = _m_psubb(
							_m_paddb(_m_paddb(v77, v84), v85),
							_m_paddb(_m_psubusb(_m_psubb(v84, _m_psubusb(v84, v83)), v85), v85));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 5:
					do
					{
						v90 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask));
						v91 = _m_paddb(v90, _m_pand(_m_psllqi(v90, 8u), (__m64)r_mask));
						v92 = _mm_cvtsi32_si64(*v12);
						v93 = _m_paddb(
							_m_paddb(_m_psrlqi(v91, 0x20u), _m_pand(_m_por(v13, v92), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v92, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v93);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(v91, _m_pand(_m_por(v93, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v93, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 6:
					do
					{
						v104 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask));
						v105 = _m_paddb(v104, _m_pand(_m_psrlqi(v104, 8u), (__m64)b_mask));
						v106 = v14;
						v107 = _mm_cvtsi32_si64(*v12);
						v108 = _m_paddb(_m_psubusb(v13, v107), v107);
						v109 = _m_psubb(v107, _m_psubusb(v107, v13));
						v110 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v105, 0x20u), v108), v109),
							_m_paddb(_m_psubusb(_m_psubb(v108, _m_psubusb(v108, v106)), v109), v109));
						*v11 = _mm_cvtsi64_si32(v110);
						v10 += v483;
						v111 = v107;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v112 = _m_paddb(_m_psubusb(v110, v14), v14);
						v113 = _m_psubb(v14, _m_psubusb(v14, v110));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(v105, v112), v113),
                            _m_paddb(_m_psubusb(_m_psubb(v112, _m_psubusb(v112, v111)), v113), v113));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 7:
					do
					{
						v118 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask));
						v119 = _m_paddb(v118, _m_pand(_m_psrlqi(v118, 8u), (__m64)b_mask));
						v120 = _mm_cvtsi32_si64(*v12);
						v121 = _m_paddb(
							_m_paddb(_m_psrlqi(v119, 0x20u), _m_pand(_m_por(v13, v120), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v120, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v121);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(v119, _m_pand(_m_por(v121, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v121, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 8:
					do
					{
						v133 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)b_mask));
						v134 = _m_paddb(v133, _m_pand(_m_psllqi(v133, 8u), (__m64)g_mask));
						v135 = _m_paddb(v134, _m_pand(_m_psllqi(v134, 8u), (__m64)r_mask));
						v136 = v14;
						v137 = _mm_cvtsi32_si64(*v12);
						v138 = _m_paddb(_m_psubusb(v13, v137), v137);
						v139 = _m_psubb(v137, _m_psubusb(v137, v13));
						v140 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v135, 0x20u), v138), v139),
							_m_paddb(_m_psubusb(_m_psubb(v138, _m_psubusb(v138, v136)), v139), v139));
						*v11 = _mm_cvtsi64_si32(v140);
						v10 += v483;
						v141 = v137;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v142 = _m_paddb(_m_psubusb(v140, v14), v14);
						v143 = _m_psubb(v14, _m_psubusb(v14, v140));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(v135, v142), v143),
                            _m_paddb(_m_psubusb(_m_psubb(v142, _m_psubusb(v142, v141)), v143), v143));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 9:
					do
					{
						v149 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)b_mask));
						v150 = _m_paddb(v149, _m_pand(_m_psllqi(v149, 8u), (__m64)g_mask));
						v151 = _m_paddb(v150, _m_pand(_m_psllqi(v150, 8u), (__m64)r_mask));
						v152 = _mm_cvtsi32_si64(*v12);
						v153 = _m_paddb(
							_m_paddb(_m_psrlqi(v151, 0x20u), _m_pand(_m_por(v13, v152), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v152, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v153);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(v151, _m_pand(_m_por(v153, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v153, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 10:
					do
					{
						v164 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)b_mask));
						v165 = _m_paddb(v164, _m_pand(_m_psllqi(v164, 8u), (__m64)g_mask));
						v166 = v14;
						v167 = _mm_cvtsi32_si64(*v12);
						v168 = _m_paddb(_m_psubusb(v13, v167), v167);
						v169 = _m_psubb(v167, _m_psubusb(v167, v13));
						v170 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v165, 0x20u), v168), v169),
							_m_paddb(_m_psubusb(_m_psubb(v168, _m_psubusb(v168, v166)), v169), v169));
						*v11 = _mm_cvtsi64_si32(v170);
						v10 += v483;
						v171 = v167;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v172 = _m_paddb(_m_psubusb(v170, v14), v14);
						v173 = _m_psubb(v14, _m_psubusb(v14, v170));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(v165, v172), v173),
                            _m_paddb(_m_psubusb(_m_psubb(v172, _m_psubusb(v172, v171)), v173), v173));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 11:
					do
					{
						v178 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)b_mask));
						v179 = _m_paddb(v178, _m_pand(_m_psllqi(v178, 8u), (__m64)g_mask));
						v180 = _mm_cvtsi32_si64(*v12);
						v181 = _m_paddb(
							_m_paddb(_m_psrlqi(v179, 0x20u), _m_pand(_m_por(v13, v180), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v180, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v181);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(v179, _m_pand(_m_por(v181, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v181, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 12:
					do
					{
						v191 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)b_mask));
						v192 = v14;
						v193 = _mm_cvtsi32_si64(*v12);
						v194 = _m_paddb(_m_psubusb(v13, v193), v193);
						v195 = _m_psubb(v193, _m_psubusb(v193, v13));
						v196 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v191, 0x20u), v194), v195),
							_m_paddb(_m_psubusb(_m_psubb(v194, _m_psubusb(v194, v192)), v195), v195));
						*v11 = _mm_cvtsi64_si32(v196);
						v10 += v483;
						v197 = v193;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v198 = _m_paddb(_m_psubusb(v196, v14), v14);
						v199 = _m_psubb(v14, _m_psubusb(v14, v196));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(v191, v198), v199),
                            _m_paddb(_m_psubusb(_m_psubb(v198, _m_psubusb(v198, v197)), v199), v199));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 13:
					do
					{
						v203 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)b_mask));
						v204 = _mm_cvtsi32_si64(*v12);
						v205 = _m_paddb(
							_m_paddb(_m_psrlqi(v203, 0x20u), _m_pand(_m_por(v13, v204), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v204, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v205);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(v203, _m_pand(_m_por(v205, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v205, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 14:
					do
					{
						v215 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask));
						v216 = v14;
						v217 = _mm_cvtsi32_si64(*v12);
						v218 = _m_paddb(_m_psubusb(v13, v217), v217);
						v219 = _m_psubb(v217, _m_psubusb(v217, v13));
						v220 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v215, 0x20u), v218), v219),
							_m_paddb(_m_psubusb(_m_psubb(v218, _m_psubusb(v218, v216)), v219), v219));
						*v11 = _mm_cvtsi64_si32(v220);
						v10 += v483;
						v221 = v217;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v222 = _m_paddb(_m_psubusb(v220, v14), v14);
						v223 = _m_psubb(v14, _m_psubusb(v14, v220));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(v215, v222), v223),
                            _m_paddb(_m_psubusb(_m_psubb(v222, _m_psubusb(v222, v221)), v223), v223));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 15:
					do
					{
						v227 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask));
						v228 = _mm_cvtsi32_si64(*v12);
						v229 = _m_paddb(
							_m_paddb(_m_psrlqi(v227, 0x20u), _m_pand(_m_por(v13, v228), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v228, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v229);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(v227, _m_pand(_m_por(v229, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v229, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 16:
					do
					{
						v239 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)r_mask));
						v240 = v14;
						v241 = _mm_cvtsi32_si64(*v12);
						v242 = _m_paddb(_m_psubusb(v13, v241), v241);
						v243 = _m_psubb(v241, _m_psubusb(v241, v13));
						v244 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v239, 0x20u), v242), v243),
							_m_paddb(_m_psubusb(_m_psubb(v242, _m_psubusb(v242, v240)), v243), v243));
						*v11 = _mm_cvtsi64_si32(v244);
						v10 += v483;
						v245 = v241;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v246 = _m_paddb(_m_psubusb(v244, v14), v14);
						v247 = _m_psubb(v14, _m_psubusb(v14, v244));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(v239, v246), v247),
                            _m_paddb(_m_psubusb(_m_psubb(v246, _m_psubusb(v246, v245)), v247), v247));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 17:
					do
					{
						v251 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)r_mask));
						v252 = _mm_cvtsi32_si64(*v12);
						v253 = _m_paddb(
							_m_paddb(_m_psrlqi(v251, 0x20u), _m_pand(_m_por(v13, v252), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v252, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v253);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(v251, _m_pand(_m_por(v253, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v253, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 18:
					do
					{
						v265 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)r_mask));
						v266 = _m_paddb(v265, _m_pand(_m_psrlqi(v265, 8u), (__m64)g_mask));
						v267 = _m_paddb(v266, _m_pand(_m_psrlqi(v266, 8u), (__m64)b_mask));
						v268 = v14;
						v269 = _mm_cvtsi32_si64(*v12);
						v270 = _m_paddb(_m_psubusb(v13, v269), v269);
						v271 = _m_psubb(v269, _m_psubusb(v269, v13));
						v272 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v267, 0x20u), v270), v271),
							_m_paddb(_m_psubusb(_m_psubb(v270, _m_psubusb(v270, v268)), v271), v271));
						*v11 = _mm_cvtsi64_si32(v272);
						v10 += v483;
						v273 = v269;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v274 = _m_paddb(_m_psubusb(v272, v14), v14);
						v275 = _m_psubb(v14, _m_psubusb(v14, v272));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(v267, v274), v275),
                            _m_paddb(_m_psubusb(_m_psubb(v274, _m_psubusb(v274, v273)), v275), v275));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 19:
					do
					{
						v281 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)r_mask));
						v282 = _m_paddb(v281, _m_pand(_m_psrlqi(v281, 8u), (__m64)g_mask));
						v283 = _m_paddb(v282, _m_pand(_m_psrlqi(v282, 8u), (__m64)b_mask));
						v284 = _mm_cvtsi32_si64(*v12);
						v285 = _m_paddb(
							_m_paddb(_m_psrlqi(v283, 0x20u), _m_pand(_m_por(v13, v284), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v284, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v285);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(v283, _m_pand(_m_por(v285, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v285, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 20:
					do
					{
						v295 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10 - 4),
								_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask)),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)b_mask));
						v296 = v14;
						v297 = _mm_cvtsi32_si64(*v12);
						v298 = _m_paddb(_m_psubusb(v13, v297), v297);
						v299 = _m_psubb(v297, _m_psubusb(v297, v13));
						v300 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v295, 0x20u), v298), v299),
							_m_paddb(_m_psubusb(_m_psubb(v298, _m_psubusb(v298, v296)), v299), v299));
						*v11 = _mm_cvtsi64_si32(v300);
						v10 += v483;
						v301 = v297;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v302 = _m_paddb(_m_psubusb(v300, v14), v14);
						v303 = _m_psubb(v14, _m_psubusb(v14, v300));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(v295, v302), v303),
                            _m_paddb(_m_psubusb(_m_psubb(v302, _m_psubusb(v302, v301)), v303), v303));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 21:
					do
					{
						v307 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10 - 4),
								_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask)),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)b_mask));
						v308 = _mm_cvtsi32_si64(*v12);
						v309 = _m_paddb(
							_m_paddb(_m_psrlqi(v307, 0x20u), _m_pand(_m_por(v13, v308), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v308, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v309);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(v307, _m_pand(_m_por(v309, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v309, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 22:
					do
					{
						v319 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10 - 4),
								_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask)),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)r_mask));
						v320 = v14;
						v321 = _mm_cvtsi32_si64(*v12);
						v322 = _m_paddb(_m_psubusb(v13, v321), v321);
						v323 = _m_psubb(v321, _m_psubusb(v321, v13));
						v324 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v319, 0x20u), v322), v323),
							_m_paddb(_m_psubusb(_m_psubb(v322, _m_psubusb(v322, v320)), v323), v323));
						*v11 = _mm_cvtsi64_si32(v324);
						v10 += v483;
						v325 = v321;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v326 = _m_paddb(_m_psubusb(v324, v14), v14);
						v327 = _m_psubb(v14, _m_psubusb(v14, v324));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(v319, v326), v327),
                            _m_paddb(_m_psubusb(_m_psubb(v326, _m_psubusb(v326, v325)), v327), v327));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 23:
					do
					{
						v331 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10 - 4),
								_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask)),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)r_mask));
						v332 = _mm_cvtsi32_si64(*v12);
						v333 = _m_paddb(
							_m_paddb(_m_psrlqi(v331, 0x20u), _m_pand(_m_por(v13, v332), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v332, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v333);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(v331, _m_pand(_m_por(v333, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v333, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 24:
					do
					{
						v348 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)r_mask));
						v349 = _m_paddb(v348, _m_pand(_m_psrlqi(v348, 8u), (__m64)g_mask));
						v350 = v14;
						v351 = _mm_cvtsi32_si64(*v12);
						v352 = _m_paddb(_m_psubusb(v13, v351), v351);
						v353 = _m_psubb(v351, _m_psubusb(v351, v13));
						v354 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v349, 0x20u), v352), v353),
							_m_paddb(_m_psubusb(_m_psubb(v352, _m_psubusb(v352, v350)), v353), v353));
						*v11 = _mm_cvtsi64_si32(v354);
						v10 += v483;
						v355 = v351;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v356 = _m_paddb(_m_psubusb(v354, v14), v14);
						v357 = _m_psubb(v14, _m_psubusb(v14, v354));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(v349, v356), v357),
                            _m_paddb(_m_psubusb(_m_psubb(v356, _m_psubusb(v356, v355)), v357), v357));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 25:
					do
					{
						v366 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)r_mask));
						v367 = _m_paddb(v366, _m_pand(_m_psrlqi(v366, 8u), (__m64)g_mask));
						v368 = _mm_cvtsi32_si64(*v12);
						v369 = _m_paddb(
							_m_paddb(_m_psrlqi(v367, 0x20u), _m_pand(_m_por(v13, v368), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v368, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v369);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(v367, _m_pand(_m_por(v369, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v369, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 26:
					do
					{
						v385 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)b_mask));
						v386 = _m_paddb(v385, _m_pand(_m_psllqi(v385, 0x10u), (__m64)r_mask));
						v387 = _m_paddb(v386, _m_pand(_m_psrlqi(v386, 8u), (__m64)g_mask));
						v388 = v14;
						v389 = _mm_cvtsi32_si64(*v12);
						v390 = _m_paddb(_m_psubusb(v13, v389), v389);
						v391 = _m_psubb(v389, _m_psubusb(v389, v13));
						v392 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v387, 0x20u), v390), v391),
							_m_paddb(_m_psubusb(_m_psubb(v390, _m_psubusb(v390, v388)), v391), v391));
						*v11 = _mm_cvtsi64_si32(v392);
						v10 += v483;
						v393 = v389;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v394 = _m_paddb(_m_psubusb(v392, v14), v14);
						v395 = _m_psubb(v14, _m_psubusb(v14, v392));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(v387, v394), v395),
                            _m_paddb(_m_psubusb(_m_psubb(v394, _m_psubusb(v394, v393)), v395), v395));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 27:
					do
					{
						v405 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)b_mask));
						v406 = _m_paddb(v405, _m_pand(_m_psllqi(v405, 0x10u), (__m64)r_mask));
						v407 = _m_paddb(v406, _m_pand(_m_psrlqi(v406, 8u), (__m64)g_mask));
						v408 = _mm_cvtsi32_si64(*v12);
						v409 = _m_paddb(
							_m_paddb(_m_psrlqi(v407, 0x20u), _m_pand(_m_por(v13, v408), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v408, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v409);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(v407, _m_pand(_m_por(v409, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v409, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 28:
					do
					{
						v425 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask));
						v426 = _m_paddb(v425, _m_pand(_m_psrlqi(v425, 8u), (__m64)b_mask));
						v427 = _m_paddb(v426, _m_pand(_m_psllqi(v426, 0x10u), (__m64)r_mask));
						v428 = v14;
						v429 = _mm_cvtsi32_si64(*v12);
						v430 = _m_paddb(_m_psubusb(v13, v429), v429);
						v431 = _m_psubb(v429, _m_psubusb(v429, v13));
						v432 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v427, 0x20u), v430), v431),
							_m_paddb(_m_psubusb(_m_psubb(v430, _m_psubusb(v430, v428)), v431), v431));
						*v11 = _mm_cvtsi64_si32(v432);
						v10 += v483;
						v433 = v429;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v434 = _m_paddb(_m_psubusb(v432, v14), v14);
						v435 = _m_psubb(v14, _m_psubusb(v14, v432));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(v427, v434), v435),
                            _m_paddb(_m_psubusb(_m_psubb(v434, _m_psubusb(v434, v433)), v435), v435));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 29:
					do
					{
						v445 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask));
						v446 = _m_paddb(v445, _m_pand(_m_psrlqi(v445, 8u), (__m64)b_mask));
						v447 = _m_paddb(v446, _m_pand(_m_psllqi(v446, 0x10u), (__m64)r_mask));
						v448 = _mm_cvtsi32_si64(*v12);
						v449 = _m_paddb(
							_m_paddb(_m_psrlqi(v447, 0x20u), _m_pand(_m_por(v13, v448), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v448, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v449);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(v447, _m_pand(_m_por(v449, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v449, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 30:
					do
					{
						v463 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10 - 4),
								_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 9u), (__m64)g_d_mask)),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 0x11u), (__m64)r_d_mask));
						v464 = v14;
						v465 = _mm_cvtsi32_si64(*v12);
						v466 = _m_paddb(_m_psubusb(v13, v465), v465);
						v467 = _m_psubb(v465, _m_psubusb(v465, v13));
						v468 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v463, 0x20u), v466), v467),
							_m_paddb(_m_psubusb(_m_psubb(v466, _m_psubusb(v466, v464)), v467), v467));
						*v11 = _mm_cvtsi64_si32(v468);
						v10 += v483;
						v469 = v465;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v470 = _m_paddb(_m_psubusb(v468, v14), v14);
						v471 = _m_psubb(v14, _m_psubusb(v14, v468));
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(v463, v470), v471),
                            _m_paddb(_m_psubusb(_m_psubb(v470, _m_psubusb(v470, v469)), v471), v471));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 31:
					do
					{
						v479 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10 - 4),
								_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 9u), (__m64)g_d_mask)),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 0x11u), (__m64)r_d_mask));
						v480 = _mm_cvtsi32_si64(*v12);
						v481 = _m_paddb(
							_m_paddb(_m_psrlqi(v479, 0x20u), _m_pand(_m_por(v13, v480), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v13, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v480, (__m64)avg_mask_fe), 1u)));
						*v11 = _mm_cvtsi64_si32(v481);
						v10 += v483;
						v14 = _mm_cvtsi32_si64(v12[1]);
						v13 = _m_paddb(
							_m_paddb(v479, _m_pand(_m_por(v481, v14), (__m64)avg_mask_01)),
							_m_paddb(
								_m_psrlqi(_m_pand(v481, (__m64)avg_mask_fe), 1u),
								_m_psrlqi(_m_pand(v14, (__m64)avg_mask_fe), 1u)));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
			}
		}
		break;
	}
	_m_empty();
}

void __cdecl TVPTLG6DecodeLine_sse_c(tjs_uint32 *prevline, tjs_uint32 *curline, tjs_int width, tjs_int block_count, tjs_uint8 *filtertypes, tjs_int skipblockbytes, tjs_uint32 *input, tjs_uint32 initialp, tjs_int oddskip, tjs_int dir)
{
	int          v10;             // edx
	tjs_uint32 * v11;             // edi
	tjs_uint32 * v12;             // esi
	__m64        v13;             // mm0
	__m64        v14;             // mm6
	tjs_uint32 * v15;             // eax
	int          v16;             // ecx
	signed int   v17;             // ebx
	__m64        v18;             // mm4
	__m64        v19;             // mm2
	__m64        v20;             // mm6
	__m64        v21;             // mm3
	__m64        v22;             // mm0
	__m64        v23;             // mm1
	__m64        v24;             // mm7
	__m64        v25;             // mm2
	__m64        v26;             // mm0
	__m64        v27;             // mm1
	__m64        v28;             // mm4
	__m64        v29;             // mm2
	__m64        v30;             // mm6
	__m64        v31;             // mm3
	__m64        v32;             // mm0
	__m64        v33;             // mm1
	__m64        v34;             // mm7
	__m64        v35;             // mm2
	__m64        v36;             // mm0
	__m64        v37;             // mm1
	__m64        v38;             // mm4
	__m64        v39;             // mm7
	__m64        v40;             // mm4
	__m64        v41;             // mm7
	__m64        v42;             // mm4
	__m64        v43;             // mm2
	__m64        v44;             // mm6
	__m64        v45;             // mm3
	__m64        v46;             // mm0
	__m64        v47;             // mm1
	__m64        v48;             // mm7
	__m64        v49;             // mm2
	__m64        v50;             // mm0
	__m64        v51;             // mm1
	__m64        v52;             // mm4
	__m64        v53;             // mm2
	__m64        v54;             // mm6
	__m64        v55;             // mm3
	__m64        v56;             // mm0
	__m64        v57;             // mm1
	__m64        v58;             // mm7
	__m64        v59;             // mm2
	__m64        v60;             // mm0
	__m64        v61;             // mm1
	__m64        v62;             // mm4
	__m64        v63;             // mm7
	__m64        v64;             // mm4
	__m64        v65;             // mm7
	__m64        v66;             // mm4
	__m64        v67;             // mm4
	__m64        v68;             // mm2
	__m64        v69;             // mm6
	__m64        v70;             // mm3
	__m64        v71;             // mm0
	__m64        v72;             // mm1
	__m64        v73;             // mm7
	__m64        v74;             // mm2
	__m64        v75;             // mm0
	__m64        v76;             // mm1
	__m64        v77;             // mm4
	__m64        v78;             // mm4
	__m64        v79;             // mm2
	__m64        v80;             // mm6
	__m64        v81;             // mm3
	__m64        v82;             // mm0
	__m64        v83;             // mm1
	__m64        v84;             // mm7
	__m64        v85;             // mm2
	__m64        v86;             // mm0
	__m64        v87;             // mm1
	__m64        v88;             // mm4
	__m64        v89;             // mm4
	__m64        v90;             // mm7
	__m64        v91;             // mm4
	__m64        v92;             // mm4
	__m64        v93;             // mm7
	__m64        v94;             // mm4
	__m64        v95;             // mm4
	__m64        v96;             // mm2
	__m64        v97;             // mm6
	__m64        v98;             // mm3
	__m64        v99;             // mm0
	__m64        v100;            // mm1
	__m64        v101;            // mm7
	__m64        v102;            // mm2
	__m64        v103;            // mm0
	__m64        v104;            // mm1
	__m64        v105;            // mm4
	__m64        v106;            // mm4
	__m64        v107;            // mm2
	__m64        v108;            // mm6
	__m64        v109;            // mm3
	__m64        v110;            // mm0
	__m64        v111;            // mm1
	__m64        v112;            // mm7
	__m64        v113;            // mm2
	__m64        v114;            // mm0
	__m64        v115;            // mm1
	__m64        v116;            // mm4
	__m64        v117;            // mm4
	__m64        v118;            // mm7
	__m64        v119;            // mm4
	__m64        v120;            // mm4
	__m64        v121;            // mm7
	__m64        v122;            // mm4
	__m64        v123;            // mm4
	__m64        v124;            // mm4
	__m64        v125;            // mm2
	__m64        v126;            // mm6
	__m64        v127;            // mm3
	__m64        v128;            // mm0
	__m64        v129;            // mm1
	__m64        v130;            // mm7
	__m64        v131;            // mm2
	__m64        v132;            // mm0
	__m64        v133;            // mm1
	__m64        v134;            // mm4
	__m64        v135;            // mm4
	__m64        v136;            // mm4
	__m64        v137;            // mm2
	__m64        v138;            // mm6
	__m64        v139;            // mm3
	__m64        v140;            // mm0
	__m64        v141;            // mm1
	__m64        v142;            // mm7
	__m64        v143;            // mm2
	__m64        v144;            // mm0
	__m64        v145;            // mm1
	__m64        v146;            // mm4
	__m64        v147;            // mm4
	__m64        v148;            // mm4
	__m64        v149;            // mm7
	__m64        v150;            // mm4
	__m64        v151;            // mm4
	__m64        v152;            // mm4
	__m64        v153;            // mm7
	__m64        v154;            // mm4
	__m64        v155;            // mm4
	__m64        v156;            // mm2
	__m64        v157;            // mm6
	__m64        v158;            // mm3
	__m64        v159;            // mm0
	__m64        v160;            // mm1
	__m64        v161;            // mm7
	__m64        v162;            // mm2
	__m64        v163;            // mm0
	__m64        v164;            // mm1
	__m64        v165;            // mm4
	__m64        v166;            // mm4
	__m64        v167;            // mm2
	__m64        v168;            // mm6
	__m64        v169;            // mm3
	__m64        v170;            // mm0
	__m64        v171;            // mm1
	__m64        v172;            // mm7
	__m64        v173;            // mm2
	__m64        v174;            // mm0
	__m64        v175;            // mm1
	__m64        v176;            // mm4
	__m64        v177;            // mm4
	__m64        v178;            // mm7
	__m64        v179;            // mm4
	__m64        v180;            // mm4
	__m64        v181;            // mm7
	__m64        v182;            // mm4
	__m64        v183;            // mm2
	__m64        v184;            // mm6
	__m64        v185;            // mm3
	__m64        v186;            // mm0
	__m64        v187;            // mm1
	__m64        v188;            // mm7
	__m64        v189;            // mm2
	__m64        v190;            // mm0
	__m64        v191;            // mm1
	__m64        v192;            // mm4
	__m64        v193;            // mm2
	__m64        v194;            // mm6
	__m64        v195;            // mm3
	__m64        v196;            // mm0
	__m64        v197;            // mm1
	__m64        v198;            // mm7
	__m64        v199;            // mm2
	__m64        v200;            // mm0
	__m64        v201;            // mm1
	__m64        v202;            // mm4
	__m64        v203;            // mm7
	__m64        v204;            // mm4
	__m64        v205;            // mm7
	__m64        v206;            // mm4
	__m64        v207;            // mm2
	__m64        v208;            // mm6
	__m64        v209;            // mm3
	__m64        v210;            // mm0
	__m64        v211;            // mm1
	__m64        v212;            // mm7
	__m64        v213;            // mm2
	__m64        v214;            // mm0
	__m64        v215;            // mm1
	__m64        v216;            // mm4
	__m64        v217;            // mm2
	__m64        v218;            // mm6
	__m64        v219;            // mm3
	__m64        v220;            // mm0
	__m64        v221;            // mm1
	__m64        v222;            // mm7
	__m64        v223;            // mm2
	__m64        v224;            // mm0
	__m64        v225;            // mm1
	__m64        v226;            // mm4
	__m64        v227;            // mm7
	__m64        v228;            // mm4
	__m64        v229;            // mm7
	__m64        v230;            // mm4
	__m64        v231;            // mm2
	__m64        v232;            // mm6
	__m64        v233;            // mm3
	__m64        v234;            // mm0
	__m64        v235;            // mm1
	__m64        v236;            // mm7
	__m64        v237;            // mm2
	__m64        v238;            // mm0
	__m64        v239;            // mm1
	__m64        v240;            // mm4
	__m64        v241;            // mm2
	__m64        v242;            // mm6
	__m64        v243;            // mm3
	__m64        v244;            // mm0
	__m64        v245;            // mm1
	__m64        v246;            // mm7
	__m64        v247;            // mm2
	__m64        v248;            // mm0
	__m64        v249;            // mm1
	__m64        v250;            // mm4
	__m64        v251;            // mm7
	__m64        v252;            // mm4
	__m64        v253;            // mm7
	__m64        v254;            // mm4
	__m64        v255;            // mm4
	__m64        v256;            // mm4
	__m64        v257;            // mm2
	__m64        v258;            // mm6
	__m64        v259;            // mm3
	__m64        v260;            // mm0
	__m64        v261;            // mm1
	__m64        v262;            // mm7
	__m64        v263;            // mm2
	__m64        v264;            // mm0
	__m64        v265;            // mm1
	__m64        v266;            // mm4
	__m64        v267;            // mm4
	__m64        v268;            // mm4
	__m64        v269;            // mm2
	__m64        v270;            // mm6
	__m64        v271;            // mm3
	__m64        v272;            // mm0
	__m64        v273;            // mm1
	__m64        v274;            // mm7
	__m64        v275;            // mm2
	__m64        v276;            // mm0
	__m64        v277;            // mm1
	__m64        v278;            // mm4
	__m64        v279;            // mm4
	__m64        v280;            // mm4
	__m64        v281;            // mm7
	__m64        v282;            // mm4
	__m64        v283;            // mm4
	__m64        v284;            // mm4
	__m64        v285;            // mm7
	__m64        v286;            // mm4
	__m64        v287;            // mm2
	__m64        v288;            // mm6
	__m64        v289;            // mm3
	__m64        v290;            // mm0
	__m64        v291;            // mm1
	__m64        v292;            // mm7
	__m64        v293;            // mm2
	__m64        v294;            // mm0
	__m64        v295;            // mm1
	__m64        v296;            // mm4
	__m64        v297;            // mm2
	__m64        v298;            // mm6
	__m64        v299;            // mm3
	__m64        v300;            // mm0
	__m64        v301;            // mm1
	__m64        v302;            // mm7
	__m64        v303;            // mm2
	__m64        v304;            // mm0
	__m64        v305;            // mm1
	__m64        v306;            // mm4
	__m64        v307;            // mm7
	__m64        v308;            // mm4
	__m64        v309;            // mm7
	__m64        v310;            // mm4
	__m64        v311;            // mm2
	__m64        v312;            // mm6
	__m64        v313;            // mm3
	__m64        v314;            // mm0
	__m64        v315;            // mm1
	__m64        v316;            // mm7
	__m64        v317;            // mm2
	__m64        v318;            // mm0
	__m64        v319;            // mm1
	__m64        v320;            // mm4
	__m64        v321;            // mm2
	__m64        v322;            // mm6
	__m64        v323;            // mm3
	__m64        v324;            // mm0
	__m64        v325;            // mm1
	__m64        v326;            // mm7
	__m64        v327;            // mm2
	__m64        v328;            // mm0
	__m64        v329;            // mm1
	__m64        v330;            // mm4
	__m64        v331;            // mm7
	__m64        v332;            // mm4
	__m64        v333;            // mm7
	__m64        v334;            // mm4
	__m64        v335;            // mm4
	__m64        v336;            // mm2
	__m64        v337;            // mm6
	__m64        v338;            // mm3
	__m64        v339;            // mm0
	__m64        v340;            // mm1
	__m64        v341;            // mm7
	__m64        v342;            // mm2
	__m64        v343;            // mm0
	__m64        v344;            // mm1
	__m64        v345;            // mm4
	__m64        v346;            // mm4
	__m64        v347;            // mm2
	__m64        v348;            // mm6
	__m64        v349;            // mm3
	__m64        v350;            // mm0
	__m64        v351;            // mm1
	__m64        v352;            // mm7
	__m64        v353;            // mm2
	__m64        v354;            // mm0
	__m64        v355;            // mm1
	__m64        v356;            // mm4
	__m64        v357;            // mm4
	__m64        v358;            // mm7
	__m64        v359;            // mm4
	__m64        v360;            // mm4
	__m64        v361;            // mm7
	__m64        v362;            // mm4
	__m64        v363;            // mm4
	__m64        v364;            // mm4
	__m64        v365;            // mm2
	__m64        v366;            // mm6
	__m64        v367;            // mm3
	__m64        v368;            // mm0
	__m64        v369;            // mm1
	__m64        v370;            // mm7
	__m64        v371;            // mm2
	__m64        v372;            // mm0
	__m64        v373;            // mm1
	__m64        v374;            // mm4
	__m64        v375;            // mm4
	__m64        v376;            // mm4
	__m64        v377;            // mm2
	__m64        v378;            // mm6
	__m64        v379;            // mm3
	__m64        v380;            // mm0
	__m64        v381;            // mm1
	__m64        v382;            // mm7
	__m64        v383;            // mm2
	__m64        v384;            // mm0
	__m64        v385;            // mm1
	__m64        v386;            // mm4
	__m64        v387;            // mm4
	__m64        v388;            // mm4
	__m64        v389;            // mm7
	__m64        v390;            // mm4
	__m64        v391;            // mm4
	__m64        v392;            // mm4
	__m64        v393;            // mm7
	__m64        v394;            // mm4
	__m64        v395;            // mm4
	__m64        v396;            // mm4
	__m64        v397;            // mm2
	__m64        v398;            // mm6
	__m64        v399;            // mm3
	__m64        v400;            // mm0
	__m64        v401;            // mm1
	__m64        v402;            // mm7
	__m64        v403;            // mm2
	__m64        v404;            // mm0
	__m64        v405;            // mm1
	__m64        v406;            // mm4
	__m64        v407;            // mm4
	__m64        v408;            // mm4
	__m64        v409;            // mm2
	__m64        v410;            // mm6
	__m64        v411;            // mm3
	__m64        v412;            // mm0
	__m64        v413;            // mm1
	__m64        v414;            // mm7
	__m64        v415;            // mm2
	__m64        v416;            // mm0
	__m64        v417;            // mm1
	__m64        v418;            // mm4
	__m64        v419;            // mm4
	__m64        v420;            // mm4
	__m64        v421;            // mm7
	__m64        v422;            // mm4
	__m64        v423;            // mm4
	__m64        v424;            // mm4
	__m64        v425;            // mm7
	__m64        v426;            // mm4
	__m64        v427;            // mm2
	__m64        v428;            // mm6
	__m64        v429;            // mm3
	__m64        v430;            // mm0
	__m64        v431;            // mm1
	__m64        v432;            // mm7
	__m64        v433;            // mm2
	__m64        v434;            // mm0
	__m64        v435;            // mm1
	__m64        v436;            // mm4
	__m64        v437;            // mm2
	__m64        v438;            // mm6
	__m64        v439;            // mm3
	__m64        v440;            // mm0
	__m64        v441;            // mm1
	__m64        v442;            // mm7
	__m64        v443;            // mm2
	__m64        v444;            // mm0
	__m64        v445;            // mm1
	__m64        v446;            // mm4
	__m64        v447;            // mm7
	__m64        v448;            // mm4
	__m64        v449;            // mm7
	char         v450;            // [esp+0h] [ebp-Ch]
	signed int   v451;            // [esp+4h] [ebp-8h]
	tjs_int      skipblockbytesa; // [esp+3Ch] [ebp+30h]
	unsigned int oddskipa;        // [esp+48h] [ebp+3Ch]

	if (dir & 1)
	{
		v451            = 8;
		skipblockbytesa = 4 * skipblockbytes - 32;
	}
	else
	{
		v451            = -8;
		skipblockbytesa = 4 * skipblockbytes + 32;
		input += 7;
	}
	oddskipa = 32 * oddskip;
	v10      = 0;
	v450     = 0;
	v11      = curline;
	v12      = prevline;
	v13      = _mm_cvtsi32_si64(initialp);
	v14      = v13;
	v15      = &input[oddskipa / 4];
	while (2)
	{
		v16 = *filtertypes;
		v17 = 4;
		v450 ^= 1u;
		if (v450)
			v15 = (tjs_uint32 *)((char *)v15 - oddskipa);
		else
			v15 = (tjs_uint32 *)((char *)v15 + oddskipa);
		++filtertypes;
		if (dir & 1)
		{
			switch (v16)
			{
				case 0:
					do
					{
						v18 = *(__m64 *)((char *)v15 + v10);
						v19 = v14;
						v20 = _mm_cvtsi32_si64(*v12);
						v21 = v13;
						v22 = _m_pmaxub(v13, v20);
						v23 = _m_pminub(v20, v21);
						v24 = _m_psubb(
							_m_paddb(_m_paddb(*(__m64 *)((char *)v15 + v10), v22), v23),
							_m_pmaxub(_m_pminub(v22, v19), v23));
						*v11 = _mm_cvtsi64_si32(v24);
						v10 += v451;
						v25    = v20;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v26    = _m_pmaxub(v24, v14);
						v27    = _m_pminub(v14, v24);
						v13    = _m_psubb(_m_paddb(_m_paddb(_m_psrlqi(v18, 0x20u), v26), v27), _m_pmaxub(_m_pminub(v26, v25), v27));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 1:
					do
					{
						v38  = *(__m64 *)((char *)v15 + v10);
						v39  = _m_paddb(*(__m64 *)((char *)v15 + v10), _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v39);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(_m_psrlqi(v38, 0x20u), _m_pavgb(v39, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 2:
					do
					{
						v42 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10),
								_m_psllqi(_m_pand(*(__m64 *)((char *)v15 + v10), (__m64)g_mask), 8u)),
							_m_psrlqi(_m_pand(*(__m64 *)((char *)v15 + v10), (__m64)g_mask), 8u));
						v43  = v14;
						v44  = _mm_cvtsi32_si64(*v12);
						v45  = v13;
						v46  = _m_pmaxub(v13, v44);
						v47  = _m_pminub(v44, v45);
						v48  = _m_psubb(_m_paddb(_m_paddb(v42, v46), v47), _m_pmaxub(_m_pminub(v46, v43), v47));
						*v11 = _mm_cvtsi64_si32(v48);
						v10 += v451;
						v49    = v44;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v50    = _m_pmaxub(v48, v14);
						v51    = _m_pminub(v14, v48);
						v13    = _m_psubb(_m_paddb(_m_paddb(_m_psrlqi(v42, 0x20u), v50), v51), _m_pmaxub(_m_pminub(v50, v49), v51));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 3:
					do
					{
						v62 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10),
								_m_psllqi(_m_pand(*(__m64 *)((char *)v15 + v10), (__m64)g_mask), 8u)),
							_m_psrlqi(_m_pand(*(__m64 *)((char *)v15 + v10), (__m64)g_mask), 8u));
						v63  = _m_paddb(v62, _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v63);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(_m_psrlqi(v62, 0x20u), _m_pavgb(v63, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 4:
					do
					{
						v66 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask));
						v67  = _m_paddb(v66, _m_pand(_m_psllqi(v66, 8u), (__m64)r_mask));
						v68  = v14;
						v69  = _mm_cvtsi32_si64(*v12);
						v70  = v13;
						v71  = _m_pmaxub(v13, v69);
						v72  = _m_pminub(v69, v70);
						v73  = _m_psubb(_m_paddb(_m_paddb(v67, v71), v72), _m_pmaxub(_m_pminub(v71, v68), v72));
						*v11 = _mm_cvtsi64_si32(v73);
						v10 += v451;
						v74    = v69;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v75    = _m_pmaxub(v73, v14);
						v76    = _m_pminub(v14, v73);
						v13    = _m_psubb(_m_paddb(_m_paddb(_m_psrlqi(v67, 0x20u), v75), v76), _m_pmaxub(_m_pminub(v75, v74), v76));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 5:
					do
					{
						v88 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask));
						v89  = _m_paddb(v88, _m_pand(_m_psllqi(v88, 8u), (__m64)r_mask));
						v90  = _m_paddb(v89, _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v90);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(_m_psrlqi(v89, 0x20u), _m_pavgb(v90, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 6:
					do
					{
						v94 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask));
						v95  = _m_paddb(v94, _m_pand(_m_psrlqi(v94, 8u), (__m64)b_mask));
						v96  = v14;
						v97  = _mm_cvtsi32_si64(*v12);
						v98  = v13;
						v99  = _m_pmaxub(v13, v97);
						v100 = _m_pminub(v97, v98);
						v101 = _m_psubb(_m_paddb(_m_paddb(v95, v99), v100), _m_pmaxub(_m_pminub(v99, v96), v100));
						*v11 = _mm_cvtsi64_si32(v101);
						v10 += v451;
						v102 = v97;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v103 = _m_pmaxub(v101, v14);
						v104 = _m_pminub(v14, v101);
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v95, 0x20u), v103), v104),
                            _m_pmaxub(_m_pminub(v103, v102), v104));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 7:
					do
					{
						v116 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask));
						v117 = _m_paddb(v116, _m_pand(_m_psrlqi(v116, 8u), (__m64)b_mask));
						v118 = _m_paddb(v117, _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v118);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(_m_psrlqi(v117, 0x20u), _m_pavgb(v118, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 8:
					do
					{
						v122 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)b_mask));
						v123 = _m_paddb(v122, _m_pand(_m_psllqi(v122, 8u), (__m64)g_mask));
						v124 = _m_paddb(v123, _m_pand(_m_psllqi(v123, 8u), (__m64)r_mask));
						v125 = v14;
						v126 = _mm_cvtsi32_si64(*v12);
						v127 = v13;
						v128 = _m_pmaxub(v13, v126);
						v129 = _m_pminub(v126, v127);
						v130 = _m_psubb(_m_paddb(_m_paddb(v124, v128), v129), _m_pmaxub(_m_pminub(v128, v125), v129));
						*v11 = _mm_cvtsi64_si32(v130);
						v10 += v451;
						v131 = v126;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v132 = _m_pmaxub(v130, v14);
						v133 = _m_pminub(v14, v130);
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v124, 0x20u), v132), v133),
                            _m_pmaxub(_m_pminub(v132, v131), v133));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 9:
					do
					{
						v146 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)b_mask));
						v147 = _m_paddb(v146, _m_pand(_m_psllqi(v146, 8u), (__m64)g_mask));
						v148 = _m_paddb(v147, _m_pand(_m_psllqi(v147, 8u), (__m64)r_mask));
						v149 = _m_paddb(v148, _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v149);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(_m_psrlqi(v148, 0x20u), _m_pavgb(v149, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 10:
					do
					{
						v154 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)b_mask));
						v155 = _m_paddb(v154, _m_pand(_m_psllqi(v154, 8u), (__m64)g_mask));
						v156 = v14;
						v157 = _mm_cvtsi32_si64(*v12);
						v158 = v13;
						v159 = _m_pmaxub(v13, v157);
						v160 = _m_pminub(v157, v158);
						v161 = _m_psubb(_m_paddb(_m_paddb(v155, v159), v160), _m_pmaxub(_m_pminub(v159, v156), v160));
						*v11 = _mm_cvtsi64_si32(v161);
						v10 += v451;
						v162 = v157;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v163 = _m_pmaxub(v161, v14);
						v164 = _m_pminub(v14, v161);
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v155, 0x20u), v163), v164),
                            _m_pmaxub(_m_pminub(v163, v162), v164));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 11:
					do
					{
						v176 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)b_mask));
						v177 = _m_paddb(v176, _m_pand(_m_psllqi(v176, 8u), (__m64)g_mask));
						v178 = _m_paddb(v177, _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v178);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(_m_psrlqi(v177, 0x20u), _m_pavgb(v178, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 12:
					do
					{
						v182 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)b_mask));
						v183 = v14;
						v184 = _mm_cvtsi32_si64(*v12);
						v185 = v13;
						v186 = _m_pmaxub(v13, v184);
						v187 = _m_pminub(v184, v185);
						v188 = _m_psubb(_m_paddb(_m_paddb(v182, v186), v187), _m_pmaxub(_m_pminub(v186, v183), v187));
						*v11 = _mm_cvtsi64_si32(v188);
						v10 += v451;
						v189 = v184;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v190 = _m_pmaxub(v188, v14);
						v191 = _m_pminub(v14, v188);
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v182, 0x20u), v190), v191),
                            _m_pmaxub(_m_pminub(v190, v189), v191));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 13:
					do
					{
						v202 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)b_mask));
						v203 = _m_paddb(v202, _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v203);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(_m_psrlqi(v202, 0x20u), _m_pavgb(v203, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 14:
					do
					{
						v206 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask));
						v207 = v14;
						v208 = _mm_cvtsi32_si64(*v12);
						v209 = v13;
						v210 = _m_pmaxub(v13, v208);
						v211 = _m_pminub(v208, v209);
						v212 = _m_psubb(_m_paddb(_m_paddb(v206, v210), v211), _m_pmaxub(_m_pminub(v210, v207), v211));
						*v11 = _mm_cvtsi64_si32(v212);
						v10 += v451;
						v213 = v208;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v214 = _m_pmaxub(v212, v14);
						v215 = _m_pminub(v14, v212);
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v206, 0x20u), v214), v215),
                            _m_pmaxub(_m_pminub(v214, v213), v215));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 15:
					do
					{
						v226 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask));
						v227 = _m_paddb(v226, _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v227);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(_m_psrlqi(v226, 0x20u), _m_pavgb(v227, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 16:
					do
					{
						v230 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)r_mask));
						v231 = v14;
						v232 = _mm_cvtsi32_si64(*v12);
						v233 = v13;
						v234 = _m_pmaxub(v13, v232);
						v235 = _m_pminub(v232, v233);
						v236 = _m_psubb(_m_paddb(_m_paddb(v230, v234), v235), _m_pmaxub(_m_pminub(v234, v231), v235));
						*v11 = _mm_cvtsi64_si32(v236);
						v10 += v451;
						v237 = v232;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v238 = _m_pmaxub(v236, v14);
						v239 = _m_pminub(v14, v236);
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v230, 0x20u), v238), v239),
                            _m_pmaxub(_m_pminub(v238, v237), v239));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 17:
					do
					{
						v250 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)r_mask));
						v251 = _m_paddb(v250, _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v251);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(_m_psrlqi(v250, 0x20u), _m_pavgb(v251, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 18:
					do
					{
						v254 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)r_mask));
						v255 = _m_paddb(v254, _m_pand(_m_psrlqi(v254, 8u), (__m64)g_mask));
						v256 = _m_paddb(v255, _m_pand(_m_psrlqi(v255, 8u), (__m64)b_mask));
						v257 = v14;
						v258 = _mm_cvtsi32_si64(*v12);
						v259 = v13;
						v260 = _m_pmaxub(v13, v258);
						v261 = _m_pminub(v258, v259);
						v262 = _m_psubb(_m_paddb(_m_paddb(v256, v260), v261), _m_pmaxub(_m_pminub(v260, v257), v261));
						*v11 = _mm_cvtsi64_si32(v262);
						v10 += v451;
						v263 = v258;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v264 = _m_pmaxub(v262, v14);
						v265 = _m_pminub(v14, v262);
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v256, 0x20u), v264), v265),
                            _m_pmaxub(_m_pminub(v264, v263), v265));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 19:
					do
					{
						v278 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)r_mask));
						v279 = _m_paddb(v278, _m_pand(_m_psrlqi(v278, 8u), (__m64)g_mask));
						v280 = _m_paddb(v279, _m_pand(_m_psrlqi(v279, 8u), (__m64)b_mask));
						v281 = _m_paddb(v280, _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v281);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(_m_psrlqi(v280, 0x20u), _m_pavgb(v281, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 20:
					do
					{
						v286 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10),
								_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask)),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)b_mask));
						v287 = v14;
						v288 = _mm_cvtsi32_si64(*v12);
						v289 = v13;
						v290 = _m_pmaxub(v13, v288);
						v291 = _m_pminub(v288, v289);
						v292 = _m_psubb(_m_paddb(_m_paddb(v286, v290), v291), _m_pmaxub(_m_pminub(v290, v287), v291));
						*v11 = _mm_cvtsi64_si32(v292);
						v10 += v451;
						v293 = v288;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v294 = _m_pmaxub(v292, v14);
						v295 = _m_pminub(v14, v292);
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v286, 0x20u), v294), v295),
                            _m_pmaxub(_m_pminub(v294, v293), v295));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 21:
					do
					{
						v306 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10),
								_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask)),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)b_mask));
						v307 = _m_paddb(v306, _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v307);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(_m_psrlqi(v306, 0x20u), _m_pavgb(v307, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 22:
					do
					{
						v310 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10),
								_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask)),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)r_mask));
						v311 = v14;
						v312 = _mm_cvtsi32_si64(*v12);
						v313 = v13;
						v314 = _m_pmaxub(v13, v312);
						v315 = _m_pminub(v312, v313);
						v316 = _m_psubb(_m_paddb(_m_paddb(v310, v314), v315), _m_pmaxub(_m_pminub(v314, v311), v315));
						*v11 = _mm_cvtsi64_si32(v316);
						v10 += v451;
						v317 = v312;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v318 = _m_pmaxub(v316, v14);
						v319 = _m_pminub(v14, v316);
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v310, 0x20u), v318), v319),
                            _m_pmaxub(_m_pminub(v318, v317), v319));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 23:
					do
					{
						v330 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10),
								_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask)),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)r_mask));
						v331 = _m_paddb(v330, _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v331);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(_m_psrlqi(v330, 0x20u), _m_pavgb(v331, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 24:
					do
					{
						v334 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)r_mask));
						v335 = _m_paddb(v334, _m_pand(_m_psrlqi(v334, 8u), (__m64)g_mask));
						v336 = v14;
						v337 = _mm_cvtsi32_si64(*v12);
						v338 = v13;
						v339 = _m_pmaxub(v13, v337);
						v340 = _m_pminub(v337, v338);
						v341 = _m_psubb(_m_paddb(_m_paddb(v335, v339), v340), _m_pmaxub(_m_pminub(v339, v336), v340));
						*v11 = _mm_cvtsi64_si32(v341);
						v10 += v451;
						v342 = v337;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v343 = _m_pmaxub(v341, v14);
						v344 = _m_pminub(v14, v341);
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v335, 0x20u), v343), v344),
                            _m_pmaxub(_m_pminub(v343, v342), v344));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 25:
					do
					{
						v356 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 0x10u), (__m64)r_mask));
						v357 = _m_paddb(v356, _m_pand(_m_psrlqi(v356, 8u), (__m64)g_mask));
						v358 = _m_paddb(v357, _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v358);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(_m_psrlqi(v357, 0x20u), _m_pavgb(v358, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 26:
					do
					{
						v362 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)b_mask));
						v363 = _m_paddb(v362, _m_pand(_m_psllqi(v362, 0x10u), (__m64)r_mask));
						v364 = _m_paddb(v363, _m_pand(_m_psrlqi(v363, 8u), (__m64)g_mask));
						v365 = v14;
						v366 = _mm_cvtsi32_si64(*v12);
						v367 = v13;
						v368 = _m_pmaxub(v13, v366);
						v369 = _m_pminub(v366, v367);
						v370 = _m_psubb(_m_paddb(_m_paddb(v364, v368), v369), _m_pmaxub(_m_pminub(v368, v365), v369));
						*v11 = _mm_cvtsi64_si32(v370);
						v10 += v451;
						v371 = v366;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v372 = _m_pmaxub(v370, v14);
						v373 = _m_pminub(v14, v370);
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v364, 0x20u), v372), v373),
                            _m_pmaxub(_m_pminub(v372, v371), v373));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 27:
					do
					{
						v386 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)b_mask));
						v387 = _m_paddb(v386, _m_pand(_m_psllqi(v386, 0x10u), (__m64)r_mask));
						v388 = _m_paddb(v387, _m_pand(_m_psrlqi(v387, 8u), (__m64)g_mask));
						v389 = _m_paddb(v388, _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v389);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(_m_psrlqi(v388, 0x20u), _m_pavgb(v389, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 28:
					do
					{
						v394 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask));
						v395 = _m_paddb(v394, _m_pand(_m_psrlqi(v394, 8u), (__m64)b_mask));
						v396 = _m_paddb(v395, _m_pand(_m_psllqi(v395, 0x10u), (__m64)r_mask));
						v397 = v14;
						v398 = _mm_cvtsi32_si64(*v12);
						v399 = v13;
						v400 = _m_pmaxub(v13, v398);
						v401 = _m_pminub(v398, v399);
						v402 = _m_psubb(_m_paddb(_m_paddb(v396, v400), v401), _m_pmaxub(_m_pminub(v400, v397), v401));
						*v11 = _mm_cvtsi64_si32(v402);
						v10 += v451;
						v403 = v398;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v404 = _m_pmaxub(v402, v14);
						v405 = _m_pminub(v14, v402);
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v396, 0x20u), v404), v405),
                            _m_pmaxub(_m_pminub(v404, v403), v405));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 29:
					do
					{
						v418 = _m_paddb(
							*(__m64 *)((char *)v15 + v10),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10), 8u), (__m64)g_mask));
						v419 = _m_paddb(v418, _m_pand(_m_psrlqi(v418, 8u), (__m64)b_mask));
						v420 = _m_paddb(v419, _m_pand(_m_psllqi(v419, 0x10u), (__m64)r_mask));
						v421 = _m_paddb(v420, _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v421);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(_m_psrlqi(v420, 0x20u), _m_pavgb(v421, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 30:
					do
					{
						v426 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10),
								_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 9u), (__m64)g_d_mask)),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 0x11u), (__m64)r_d_mask));
						v427 = v14;
						v428 = _mm_cvtsi32_si64(*v12);
						v429 = v13;
						v430 = _m_pmaxub(v13, v428);
						v431 = _m_pminub(v428, v429);
						v432 = _m_psubb(_m_paddb(_m_paddb(v426, v430), v431), _m_pmaxub(_m_pminub(v430, v427), v431));
						*v11 = _mm_cvtsi64_si32(v432);
						v10 += v451;
						v433 = v428;
						v14  = _mm_cvtsi32_si64(v12[1]);
						v434 = _m_pmaxub(v432, v14);
						v435 = _m_pminub(v14, v432);
						v13  = _m_psubb(
                            _m_paddb(_m_paddb(_m_psrlqi(v426, 0x20u), v434), v435),
                            _m_pmaxub(_m_pminub(v434, v433), v435));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 31:
					do
					{
						v446 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10),
								_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 9u), (__m64)g_d_mask)),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10), 0x11u), (__m64)r_d_mask));
						v447 = _m_paddb(v446, _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v447);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(_m_psrlqi(v446, 0x20u), _m_pavgb(v447, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
			}
		}
		else
		{
			switch (v16)
			{
				case 0:
					do
					{
						v28 = *(__m64 *)((char *)v15 + v10 - 4);
						v29 = v14;
						v30 = _mm_cvtsi32_si64(*v12);
						v31 = v13;
						v32 = _m_pmaxub(v13, v30);
						v33 = _m_pminub(v30, v31);
						v34 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 0x20u), v32), v33),
							_m_pmaxub(_m_pminub(v32, v29), v33));
						*v11 = _mm_cvtsi64_si32(v34);
						v10 += v451;
						v35    = v30;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v36    = _m_pmaxub(v34, v14);
						v37    = _m_pminub(v14, v34);
						v13    = _m_psubb(_m_paddb(_m_paddb(v28, v36), v37), _m_pmaxub(_m_pminub(v36, v35), v37));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 1:
					do
					{
						v40  = *(__m64 *)((char *)v15 + v10 - 4);
						v41  = _m_paddb(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 0x20u), _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v41);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(v40, _m_pavgb(v41, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 2:
					do
					{
						v52 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10 - 4),
								_m_psllqi(_m_pand(*(__m64 *)((char *)v15 + v10 - 4), (__m64)g_mask), 8u)),
							_m_psrlqi(_m_pand(*(__m64 *)((char *)v15 + v10 - 4), (__m64)g_mask), 8u));
						v53  = v14;
						v54  = _mm_cvtsi32_si64(*v12);
						v55  = v13;
						v56  = _m_pmaxub(v13, v54);
						v57  = _m_pminub(v54, v55);
						v58  = _m_psubb(_m_paddb(_m_paddb(_m_psrlqi(v52, 0x20u), v56), v57), _m_pmaxub(_m_pminub(v56, v53), v57));
						*v11 = _mm_cvtsi64_si32(v58);
						v10 += v451;
						v59    = v54;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v60    = _m_pmaxub(v58, v14);
						v61    = _m_pminub(v14, v58);
						v13    = _m_psubb(_m_paddb(_m_paddb(v52, v60), v61), _m_pmaxub(_m_pminub(v60, v59), v61));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 3:
					do
					{
						v64 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10 - 4),
								_m_psllqi(_m_pand(*(__m64 *)((char *)v15 + v10 - 4), (__m64)g_mask), 8u)),
							_m_psrlqi(_m_pand(*(__m64 *)((char *)v15 + v10 - 4), (__m64)g_mask), 8u));
						v65  = _m_paddb(_m_psrlqi(v64, 0x20u), _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v65);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(v64, _m_pavgb(v65, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 4:
					do
					{
						v77 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask));
						v78  = _m_paddb(v77, _m_pand(_m_psllqi(v77, 8u), (__m64)r_mask));
						v79  = v14;
						v80  = _mm_cvtsi32_si64(*v12);
						v81  = v13;
						v82  = _m_pmaxub(v13, v80);
						v83  = _m_pminub(v80, v81);
						v84  = _m_psubb(_m_paddb(_m_paddb(_m_psrlqi(v78, 0x20u), v82), v83), _m_pmaxub(_m_pminub(v82, v79), v83));
						*v11 = _mm_cvtsi64_si32(v84);
						v10 += v451;
						v85    = v80;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v86    = _m_pmaxub(v84, v14);
						v87    = _m_pminub(v14, v84);
						v13    = _m_psubb(_m_paddb(_m_paddb(v78, v86), v87), _m_pmaxub(_m_pminub(v86, v85), v87));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 5:
					do
					{
						v91 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask));
						v92  = _m_paddb(v91, _m_pand(_m_psllqi(v91, 8u), (__m64)r_mask));
						v93  = _m_paddb(_m_psrlqi(v92, 0x20u), _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v93);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(v92, _m_pavgb(v93, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 6:
					do
					{
						v105 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask));
						v106 = _m_paddb(v105, _m_pand(_m_psrlqi(v105, 8u), (__m64)b_mask));
						v107 = v14;
						v108 = _mm_cvtsi32_si64(*v12);
						v109 = v13;
						v110 = _m_pmaxub(v13, v108);
						v111 = _m_pminub(v108, v109);
						v112 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v106, 0x20u), v110), v111),
							_m_pmaxub(_m_pminub(v110, v107), v111));
						*v11 = _mm_cvtsi64_si32(v112);
						v10 += v451;
						v113   = v108;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v114   = _m_pmaxub(v112, v14);
						v115   = _m_pminub(v14, v112);
						v13    = _m_psubb(_m_paddb(_m_paddb(v106, v114), v115), _m_pmaxub(_m_pminub(v114, v113), v115));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 7:
					do
					{
						v119 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask));
						v120 = _m_paddb(v119, _m_pand(_m_psrlqi(v119, 8u), (__m64)b_mask));
						v121 = _m_paddb(_m_psrlqi(v120, 0x20u), _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v121);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(v120, _m_pavgb(v121, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 8:
					do
					{
						v134 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)b_mask));
						v135 = _m_paddb(v134, _m_pand(_m_psllqi(v134, 8u), (__m64)g_mask));
						v136 = _m_paddb(v135, _m_pand(_m_psllqi(v135, 8u), (__m64)r_mask));
						v137 = v14;
						v138 = _mm_cvtsi32_si64(*v12);
						v139 = v13;
						v140 = _m_pmaxub(v13, v138);
						v141 = _m_pminub(v138, v139);
						v142 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v136, 0x20u), v140), v141),
							_m_pmaxub(_m_pminub(v140, v137), v141));
						*v11 = _mm_cvtsi64_si32(v142);
						v10 += v451;
						v143   = v138;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v144   = _m_pmaxub(v142, v14);
						v145   = _m_pminub(v14, v142);
						v13    = _m_psubb(_m_paddb(_m_paddb(v136, v144), v145), _m_pmaxub(_m_pminub(v144, v143), v145));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 9:
					do
					{
						v150 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)b_mask));
						v151 = _m_paddb(v150, _m_pand(_m_psllqi(v150, 8u), (__m64)g_mask));
						v152 = _m_paddb(v151, _m_pand(_m_psllqi(v151, 8u), (__m64)r_mask));
						v153 = _m_paddb(_m_psrlqi(v152, 0x20u), _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v153);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(v152, _m_pavgb(v153, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 10:
					do
					{
						v165 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)b_mask));
						v166 = _m_paddb(v165, _m_pand(_m_psllqi(v165, 8u), (__m64)g_mask));
						v167 = v14;
						v168 = _mm_cvtsi32_si64(*v12);
						v169 = v13;
						v170 = _m_pmaxub(v13, v168);
						v171 = _m_pminub(v168, v169);
						v172 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v166, 0x20u), v170), v171),
							_m_pmaxub(_m_pminub(v170, v167), v171));
						*v11 = _mm_cvtsi64_si32(v172);
						v10 += v451;
						v173   = v168;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v174   = _m_pmaxub(v172, v14);
						v175   = _m_pminub(v14, v172);
						v13    = _m_psubb(_m_paddb(_m_paddb(v166, v174), v175), _m_pmaxub(_m_pminub(v174, v173), v175));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 11:
					do
					{
						v179 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)b_mask));
						v180 = _m_paddb(v179, _m_pand(_m_psllqi(v179, 8u), (__m64)g_mask));
						v181 = _m_paddb(_m_psrlqi(v180, 0x20u), _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v181);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(v180, _m_pavgb(v181, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 12:
					do
					{
						v192 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)b_mask));
						v193 = v14;
						v194 = _mm_cvtsi32_si64(*v12);
						v195 = v13;
						v196 = _m_pmaxub(v13, v194);
						v197 = _m_pminub(v194, v195);
						v198 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v192, 0x20u), v196), v197),
							_m_pmaxub(_m_pminub(v196, v193), v197));
						*v11 = _mm_cvtsi64_si32(v198);
						v10 += v451;
						v199   = v194;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v200   = _m_pmaxub(v198, v14);
						v201   = _m_pminub(v14, v198);
						v13    = _m_psubb(_m_paddb(_m_paddb(v192, v200), v201), _m_pmaxub(_m_pminub(v200, v199), v201));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 13:
					do
					{
						v204 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)b_mask));
						v205 = _m_paddb(_m_psrlqi(v204, 0x20u), _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v205);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(v204, _m_pavgb(v205, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 14:
					do
					{
						v216 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask));
						v217 = v14;
						v218 = _mm_cvtsi32_si64(*v12);
						v219 = v13;
						v220 = _m_pmaxub(v13, v218);
						v221 = _m_pminub(v218, v219);
						v222 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v216, 0x20u), v220), v221),
							_m_pmaxub(_m_pminub(v220, v217), v221));
						*v11 = _mm_cvtsi64_si32(v222);
						v10 += v451;
						v223   = v218;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v224   = _m_pmaxub(v222, v14);
						v225   = _m_pminub(v14, v222);
						v13    = _m_psubb(_m_paddb(_m_paddb(v216, v224), v225), _m_pmaxub(_m_pminub(v224, v223), v225));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 15:
					do
					{
						v228 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask));
						v229 = _m_paddb(_m_psrlqi(v228, 0x20u), _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v229);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(v228, _m_pavgb(v229, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 16:
					do
					{
						v240 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)r_mask));
						v241 = v14;
						v242 = _mm_cvtsi32_si64(*v12);
						v243 = v13;
						v244 = _m_pmaxub(v13, v242);
						v245 = _m_pminub(v242, v243);
						v246 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v240, 0x20u), v244), v245),
							_m_pmaxub(_m_pminub(v244, v241), v245));
						*v11 = _mm_cvtsi64_si32(v246);
						v10 += v451;
						v247   = v242;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v248   = _m_pmaxub(v246, v14);
						v249   = _m_pminub(v14, v246);
						v13    = _m_psubb(_m_paddb(_m_paddb(v240, v248), v249), _m_pmaxub(_m_pminub(v248, v247), v249));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 17:
					do
					{
						v252 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)r_mask));
						v253 = _m_paddb(_m_psrlqi(v252, 0x20u), _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v253);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(v252, _m_pavgb(v253, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 18:
					do
					{
						v266 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)r_mask));
						v267 = _m_paddb(v266, _m_pand(_m_psrlqi(v266, 8u), (__m64)g_mask));
						v268 = _m_paddb(v267, _m_pand(_m_psrlqi(v267, 8u), (__m64)b_mask));
						v269 = v14;
						v270 = _mm_cvtsi32_si64(*v12);
						v271 = v13;
						v272 = _m_pmaxub(v13, v270);
						v273 = _m_pminub(v270, v271);
						v274 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v268, 0x20u), v272), v273),
							_m_pmaxub(_m_pminub(v272, v269), v273));
						*v11 = _mm_cvtsi64_si32(v274);
						v10 += v451;
						v275   = v270;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v276   = _m_pmaxub(v274, v14);
						v277   = _m_pminub(v14, v274);
						v13    = _m_psubb(_m_paddb(_m_paddb(v268, v276), v277), _m_pmaxub(_m_pminub(v276, v275), v277));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 19:
					do
					{
						v282 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)r_mask));
						v283 = _m_paddb(v282, _m_pand(_m_psrlqi(v282, 8u), (__m64)g_mask));
						v284 = _m_paddb(v283, _m_pand(_m_psrlqi(v283, 8u), (__m64)b_mask));
						v285 = _m_paddb(_m_psrlqi(v284, 0x20u), _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v285);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(v284, _m_pavgb(v285, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 20:
					do
					{
						v296 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10 - 4),
								_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask)),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)b_mask));
						v297 = v14;
						v298 = _mm_cvtsi32_si64(*v12);
						v299 = v13;
						v300 = _m_pmaxub(v13, v298);
						v301 = _m_pminub(v298, v299);
						v302 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v296, 0x20u), v300), v301),
							_m_pmaxub(_m_pminub(v300, v297), v301));
						*v11 = _mm_cvtsi64_si32(v302);
						v10 += v451;
						v303   = v298;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v304   = _m_pmaxub(v302, v14);
						v305   = _m_pminub(v14, v302);
						v13    = _m_psubb(_m_paddb(_m_paddb(v296, v304), v305), _m_pmaxub(_m_pminub(v304, v303), v305));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 21:
					do
					{
						v308 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10 - 4),
								_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask)),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)b_mask));
						v309 = _m_paddb(_m_psrlqi(v308, 0x20u), _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v309);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(v308, _m_pavgb(v309, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 22:
					do
					{
						v320 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10 - 4),
								_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask)),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)r_mask));
						v321 = v14;
						v322 = _mm_cvtsi32_si64(*v12);
						v323 = v13;
						v324 = _m_pmaxub(v13, v322);
						v325 = _m_pminub(v322, v323);
						v326 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v320, 0x20u), v324), v325),
							_m_pmaxub(_m_pminub(v324, v321), v325));
						*v11 = _mm_cvtsi64_si32(v326);
						v10 += v451;
						v327   = v322;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v328   = _m_pmaxub(v326, v14);
						v329   = _m_pminub(v14, v326);
						v13    = _m_psubb(_m_paddb(_m_paddb(v320, v328), v329), _m_pmaxub(_m_pminub(v328, v327), v329));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 23:
					do
					{
						v332 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10 - 4),
								_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask)),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)r_mask));
						v333 = _m_paddb(_m_psrlqi(v332, 0x20u), _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v333);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(v332, _m_pavgb(v333, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 24:
					do
					{
						v345 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)r_mask));
						v346 = _m_paddb(v345, _m_pand(_m_psrlqi(v345, 8u), (__m64)g_mask));
						v347 = v14;
						v348 = _mm_cvtsi32_si64(*v12);
						v349 = v13;
						v350 = _m_pmaxub(v13, v348);
						v351 = _m_pminub(v348, v349);
						v352 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v346, 0x20u), v350), v351),
							_m_pmaxub(_m_pminub(v350, v347), v351));
						*v11 = _mm_cvtsi64_si32(v352);
						v10 += v451;
						v353   = v348;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v354   = _m_pmaxub(v352, v14);
						v355   = _m_pminub(v14, v352);
						v13    = _m_psubb(_m_paddb(_m_paddb(v346, v354), v355), _m_pmaxub(_m_pminub(v354, v353), v355));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 25:
					do
					{
						v359 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 0x10u), (__m64)r_mask));
						v360 = _m_paddb(v359, _m_pand(_m_psrlqi(v359, 8u), (__m64)g_mask));
						v361 = _m_paddb(_m_psrlqi(v360, 0x20u), _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v361);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(v360, _m_pavgb(v361, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 26:
					do
					{
						v374 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)b_mask));
						v375 = _m_paddb(v374, _m_pand(_m_psllqi(v374, 0x10u), (__m64)r_mask));
						v376 = _m_paddb(v375, _m_pand(_m_psrlqi(v375, 8u), (__m64)g_mask));
						v377 = v14;
						v378 = _mm_cvtsi32_si64(*v12);
						v379 = v13;
						v380 = _m_pmaxub(v13, v378);
						v381 = _m_pminub(v378, v379);
						v382 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v376, 0x20u), v380), v381),
							_m_pmaxub(_m_pminub(v380, v377), v381));
						*v11 = _mm_cvtsi64_si32(v382);
						v10 += v451;
						v383   = v378;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v384   = _m_pmaxub(v382, v14);
						v385   = _m_pminub(v14, v382);
						v13    = _m_psubb(_m_paddb(_m_paddb(v376, v384), v385), _m_pmaxub(_m_pminub(v384, v383), v385));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 27:
					do
					{
						v390 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)b_mask));
						v391 = _m_paddb(v390, _m_pand(_m_psllqi(v390, 0x10u), (__m64)r_mask));
						v392 = _m_paddb(v391, _m_pand(_m_psrlqi(v391, 8u), (__m64)g_mask));
						v393 = _m_paddb(_m_psrlqi(v392, 0x20u), _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v393);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(v392, _m_pavgb(v393, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 28:
					do
					{
						v406 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask));
						v407 = _m_paddb(v406, _m_pand(_m_psrlqi(v406, 8u), (__m64)b_mask));
						v408 = _m_paddb(v407, _m_pand(_m_psllqi(v407, 0x10u), (__m64)r_mask));
						v409 = v14;
						v410 = _mm_cvtsi32_si64(*v12);
						v411 = v13;
						v412 = _m_pmaxub(v13, v410);
						v413 = _m_pminub(v410, v411);
						v414 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v408, 0x20u), v412), v413),
							_m_pmaxub(_m_pminub(v412, v409), v413));
						*v11 = _mm_cvtsi64_si32(v414);
						v10 += v451;
						v415   = v410;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v416   = _m_pmaxub(v414, v14);
						v417   = _m_pminub(v14, v414);
						v13    = _m_psubb(_m_paddb(_m_paddb(v408, v416), v417), _m_pmaxub(_m_pminub(v416, v415), v417));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 29:
					do
					{
						v422 = _m_paddb(
							*(__m64 *)((char *)v15 + v10 - 4),
							_m_pand(_m_psrlqi(*(__m64 *)((char *)v15 + v10 - 4), 8u), (__m64)g_mask));
						v423 = _m_paddb(v422, _m_pand(_m_psrlqi(v422, 8u), (__m64)b_mask));
						v424 = _m_paddb(v423, _m_pand(_m_psllqi(v423, 0x10u), (__m64)r_mask));
						v425 = _m_paddb(_m_psrlqi(v424, 0x20u), _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v425);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(v424, _m_pavgb(v425, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 30:
					do
					{
						v436 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10 - 4),
								_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 9u), (__m64)g_d_mask)),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 0x11u), (__m64)r_d_mask));
						v437 = v14;
						v438 = _mm_cvtsi32_si64(*v12);
						v439 = v13;
						v440 = _m_pmaxub(v13, v438);
						v441 = _m_pminub(v438, v439);
						v442 = _m_psubb(
							_m_paddb(_m_paddb(_m_psrlqi(v436, 0x20u), v440), v441),
							_m_pmaxub(_m_pminub(v440, v437), v441));
						*v11 = _mm_cvtsi64_si32(v442);
						v10 += v451;
						v443   = v438;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v444   = _m_pmaxub(v442, v14);
						v445   = _m_pminub(v14, v442);
						v13    = _m_psubb(_m_paddb(_m_paddb(v436, v444), v445), _m_pmaxub(_m_pminub(v444, v443), v445));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
				case 31:
					do
					{
						v448 = _m_paddb(
							_m_paddb(
								*(__m64 *)((char *)v15 + v10 - 4),
								_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 9u), (__m64)g_d_mask)),
							_m_pand(_m_psllqi(*(__m64 *)((char *)v15 + v10 - 4), 0x11u), (__m64)r_d_mask));
						v449 = _m_paddb(_m_psrlqi(v448, 0x20u), _m_pavgb(v13, _mm_cvtsi32_si64(*v12)));
						*v11 = _mm_cvtsi64_si32(v449);
						v10 += v451;
						v14    = _mm_cvtsi32_si64(v12[1]);
						v13    = _m_paddb(v448, _m_pavgb(v449, v14));
						v11[1] = _mm_cvtsi64_si32(v13);
						--v17;
						v11 += 2;
						v12 += 2;
					} while (v17);
					v10 += skipblockbytesa;
					if (--block_count)
						continue;
					break;
			}
		}
		break;
	}
	_m_empty();
}
