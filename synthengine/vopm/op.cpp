#include "op.h"
#include "global.h"
#include "globhead.h"

#define IS_ZERO_CLOSS(a,b) ((a<0 && b>=0) || (a>0 && b<=0))

const int NEXTSTAT[RELEASE_MAX+1]={
	DECAY, SUSTAIN, SUSTAIN_MAX, SUSTAIN_MAX, RELEASE_MAX, RELEASE_MAX,
};
const int MAXSTAT[RELEASE_MAX+1]={
	ATACK, SUSTAIN_MAX, SUSTAIN_MAX, SUSTAIN_MAX, RELEASE_MAX, RELEASE_MAX,
};


Op::Op(void) {
};

void Op::Init() {
	Note = 5*12+8;
	Kc = 5*16+8 + 1;
	Kf = 5;
	Ar = 10;
	D1r = 10;
	D2r = 5;
	Rr = 15;
	Ks = 1;
	Dt2 = 0;
	Dt1 = 0;

	ArTime = 0;
	Fl = 31;
	Out2Fb = 0;
	inp = 0;
	Inp_last = 0;
	DeltaT = 0;
	LfoPitch = CULC_DELTA_T;
	T = 0;
	LfoLevel = CULC_ALPHA;
	Alpha = 0;
	Tl = (128-127)<<3;
	Xr_el = 1024;
	Mul = 2;
	Ame = 0;

//	NoiseStep = (__int64)(1<<26)*(__int64)62500/Samprate;
	NoiseStep = (__int64)(1<<26)*(__int64)OpmRate/(Samprate*2);
	SetNFRQ(0);
	NoiseValue = 1;

	// 状態推移テーブルを作成
//	StatTbl[ATACK].nextstat = DECAY;
//	StatTbl[DECAY].nextstat = SUSTAIN;
//	StatTbl[SUSTAIN].nextstat = SUSTAIN_MAX;
//	StatTbl[SUSTAIN_MAX].nextstat = SUSTAIN_MAX;
//	StatTbl[RELEASE].nextstat = RELEASE_MAX;
//	StatTbl[RELEASE_MAX].nextstat = RELEASE_MAX;

	StatTbl[ATACK].limit = 0;
	StatTbl[DECAY].limit = D1LTBL[0];
	StatTbl[SUSTAIN].limit = 63;
	StatTbl[SUSTAIN_MAX].limit = 63;
	StatTbl[RELEASE].limit = 63;
	StatTbl[RELEASE_MAX].limit = 63;

	StatTbl[SUSTAIN_MAX].val_and = 4097;
	StatTbl[SUSTAIN_MAX].cmp = 2048;
	StatTbl[SUSTAIN_MAX].add = 0;
	StatTbl[RELEASE_MAX].val_and = 4097;
	StatTbl[RELEASE_MAX].cmp = 2048;
	StatTbl[RELEASE_MAX].add = 0;

	Xr_stat = RELEASE_MAX;
	Xr_and = StatTbl[Xr_stat].val_and;
	Xr_cmp = StatTbl[Xr_stat].cmp;
	Xr_add = StatTbl[Xr_stat].add;
	Xr_limit = StatTbl[Xr_stat].limit;

	CulcArStep();
	CulcD1rStep();
	CulcD2rStep();
	CulcRrStep();
	CulcPitch();
	CulcDt1Pitch();

//2006.03.26 追加 sam lfo更新タイミング修正のため
	SinBf=0;
	LfoLevelReCalc=true;

}

 void Op::InitSamprate() {
	LfoPitch = CULC_DELTA_T;

//	NoiseStep = (__int64)(1<<26)*(__int64)62500/Samprate;
	NoiseStep = (__int64)(1<<26)*(__int64)OpmRate/(Samprate*2);
	CulcNoiseCycle();

	CulcArStep();
	CulcD1rStep();
	CulcD2rStep();
	CulcRrStep();
	CulcPitch();
	CulcDt1Pitch();
}

 void	Op::CulcArStep() {
	if (Ar != 0) {
		int ks = (Ar<<1)+(Kc>>(5-Ks));
			StatTbl[ATACK].val_and = XRTBL[ks].val_and;
			StatTbl[ATACK].cmp = XRTBL[ks].val_and>>1;
		if (ks < 62) {
			StatTbl[ATACK].add = XRTBL[ks].add;
		} else {
			StatTbl[ATACK].add = 128;
		}
	} else {
		StatTbl[ATACK].val_and = 4097;
		StatTbl[ATACK].cmp = 2048;
		StatTbl[ATACK].add = 0;
	}
	if (Xr_stat == ATACK) {
		Xr_and = StatTbl[Xr_stat].val_and;
		Xr_cmp = StatTbl[Xr_stat].cmp;
		Xr_add = StatTbl[Xr_stat].add;
	}
};
 void	Op::CulcD1rStep() {
	if (D1r != 0) {
		int ks = (D1r<<1)+(Kc>>(5-Ks));
		StatTbl[DECAY].val_and = XRTBL[ks].val_and;
		StatTbl[DECAY].cmp = XRTBL[ks].val_and>>1;
		StatTbl[DECAY].add = XRTBL[ks].add;
	} else {
		StatTbl[DECAY].val_and = 4097;
		StatTbl[DECAY].cmp = 2048;
		StatTbl[DECAY].add = 0;
	}
	if (Xr_stat == DECAY) {
		Xr_and = StatTbl[Xr_stat].val_and;
		Xr_cmp = StatTbl[Xr_stat].cmp;
		Xr_add = StatTbl[Xr_stat].add;
	}
};
 void	Op::CulcD2rStep() {
	if (D2r != 0) {
		int ks = (D2r<<1)+(Kc>>(5-Ks));
		StatTbl[SUSTAIN].val_and = XRTBL[ks].val_and;
		StatTbl[SUSTAIN].cmp = XRTBL[ks].val_and>>1;
		StatTbl[SUSTAIN].add = XRTBL[ks].add;
	} else {
		StatTbl[SUSTAIN].val_and = 4097;
		StatTbl[SUSTAIN].cmp = 2048;
		StatTbl[SUSTAIN].add = 0;
	}
	if (Xr_stat == SUSTAIN) {
		Xr_and = StatTbl[Xr_stat].val_and;
		Xr_cmp = StatTbl[Xr_stat].cmp;
		Xr_add = StatTbl[Xr_stat].add;
	}
};
 void	Op::CulcRrStep() {
	int ks = (Rr<<2)+2+(Kc>>(5-Ks));
	StatTbl[RELEASE].val_and = XRTBL[ks].val_and;
	StatTbl[RELEASE].cmp = XRTBL[ks].val_and>>1;
	StatTbl[RELEASE].add = XRTBL[ks].add;
	if (Xr_stat == RELEASE) {
		Xr_and = StatTbl[Xr_stat].val_and;
		Xr_cmp = StatTbl[Xr_stat].cmp;
		Xr_add = StatTbl[Xr_stat].add;
	}
};
 void Op::CulcPitch() {
	Pitch = (Note<<6)+Kf+Dt2;
}
 void Op::CulcDt1Pitch() {
	Dt1Pitch = DT1TBL[(Kc&0xFC)+(Dt1&3)];
	if (Dt1&0x04) {
		Dt1Pitch = -Dt1Pitch;
	}
}

 void Op::SetFL(int n) {
	n = (n>>3) & 7;
	if (n == 0) {
		Fl = 31;
	} else {
		Fl = (7-n+1+1);
	}
};

 void Op::SetKC(int n) {
	Kc = n & 127;
	int note = Kc & 15;
	Note = ((Kc>>4)+1)*12+ note-(note>>2);
	++Kc;
	CulcPitch();
	CulcDt1Pitch();
	LfoPitch = CULC_DELTA_T;

	CulcArStep();
	CulcD1rStep();
	CulcD2rStep();
	CulcRrStep();
};

 void Op::SetKF(int n) {
	Kf = (n&255)>>2;
	CulcPitch();
	LfoPitch = CULC_DELTA_T;

};

 void Op::SetDT1MUL(int n) {
	Dt1 = (n>>4)&7;
	CulcDt1Pitch();
	Mul = (n&15)<<1;
	if (Mul == 0) {
		Mul = 1;
	}
	LfoPitch = CULC_DELTA_T;
};

 void Op::SetTL(int n) {
	Tl = (128-(n&127))<<3;
//	LfoLevel = CULC_ALPHA;
	LfoLevelReCalc=true;
};

 void Op::SetKSAR(int n) {
	Ks = (n&255)>>6;
	Ar = n & 31;
	CulcArStep();
	CulcD1rStep();
	CulcD2rStep();
	CulcRrStep();
};

 void Op::SetAMED1R(int n) {
	D1r = n & 31;
	CulcD1rStep();
	Ame = 0;
	if (n & 0x80) {
		Ame = -1;
	}
};

 void Op::SetDT2D2R(int n) {
	Dt2 = DT2TBL[(n&255)>>6];
	CulcPitch();
	LfoPitch = CULC_DELTA_T;
	D2r = n & 31;
	CulcD2rStep();
};

 void Op::SetD1LRR(int n) {
	StatTbl[DECAY].limit = D1LTBL[(n&255)>>4];
	if (Xr_stat == DECAY) {
		Xr_limit = StatTbl[DECAY].limit;
	}

	Rr = n & 15;
	CulcRrStep();
};

 void Op::KeyON() {
	if (Xr_stat >= RELEASE) {
		// KEYON
		T = 0;

		if (Xr_el == 0) {
			Xr_stat = DECAY;
			Xr_and = StatTbl[Xr_stat].val_and;
			Xr_cmp = StatTbl[Xr_stat].cmp;
			Xr_add = StatTbl[Xr_stat].add;
			Xr_limit = StatTbl[Xr_stat].limit;
			if ((Xr_el>>4) == Xr_limit) {
				Xr_stat = NEXTSTAT[Xr_stat];
				Xr_and = StatTbl[Xr_stat].val_and;
				Xr_cmp = StatTbl[Xr_stat].cmp;
				Xr_add = StatTbl[Xr_stat].add;
				Xr_limit = StatTbl[Xr_stat].limit;
			}
		} else {
			Xr_stat = ATACK;
			Xr_and = StatTbl[Xr_stat].val_and;
			Xr_cmp = StatTbl[Xr_stat].cmp;
			Xr_add = StatTbl[Xr_stat].add;
			Xr_limit = StatTbl[Xr_stat].limit;
		}
	}
};
 void Op::KeyOFF() {
	Xr_stat = RELEASE;
	Xr_and = StatTbl[Xr_stat].val_and;
	Xr_cmp = StatTbl[Xr_stat].cmp;
	Xr_add = StatTbl[Xr_stat].add;
	Xr_limit = StatTbl[Xr_stat].limit;
	if ((Xr_el>>4) >= 63) {
		Xr_el = 1024;
		Xr_stat = MAXSTAT[Xr_stat];
		Xr_and = StatTbl[Xr_stat].val_and;
		Xr_cmp = StatTbl[Xr_stat].cmp;
		Xr_add = StatTbl[Xr_stat].add;
		Xr_limit = StatTbl[Xr_stat].limit;
	}
};

 void Op::Envelope(int env_counter) {
        if(Xr_stat==RELEASE_MAX){return;}
	if ((env_counter&Xr_and) == Xr_cmp) {

		if (Xr_stat==ATACK) {
			// ATACK
			Xr_step += Xr_add;
			Xr_el += ((~Xr_el)*(Xr_step>>3)) >> 4;
//			LfoLevel = CULC_ALPHA;
			LfoLevelReCalc=true;

			Xr_step &= 7;

			if (Xr_el <= 0) {
				Xr_el = 0;
				Xr_stat = DECAY;
				Xr_and = StatTbl[Xr_stat].val_and;
				Xr_cmp = StatTbl[Xr_stat].cmp;
				Xr_add = StatTbl[Xr_stat].add;
				Xr_limit = StatTbl[Xr_stat].limit;
				if ((Xr_el>>4) == Xr_limit) {
					Xr_stat = NEXTSTAT[Xr_stat];
					Xr_and = StatTbl[Xr_stat].val_and;
					Xr_cmp = StatTbl[Xr_stat].cmp;
					Xr_add = StatTbl[Xr_stat].add;
					Xr_limit = StatTbl[Xr_stat].limit;
				}
			}
		} else {
			// DECAY, SUSTAIN, RELEASE
			Xr_step += Xr_add;
			Xr_el += Xr_step>>3;
//			LfoLevel = CULC_ALPHA;
			LfoLevelReCalc=true;

			Xr_step &= 7;

			int e = Xr_el>>4;
			if (e == 63) {
				Xr_el = 1024;
				Xr_stat = MAXSTAT[Xr_stat];
				Xr_and = StatTbl[Xr_stat].val_and;
				Xr_cmp = StatTbl[Xr_stat].cmp;
				Xr_add = StatTbl[Xr_stat].add;
				Xr_limit = StatTbl[Xr_stat].limit;
			} else if (e == Xr_limit) {
				Xr_stat = NEXTSTAT[Xr_stat];
				Xr_and = StatTbl[Xr_stat].val_and;
				Xr_cmp = StatTbl[Xr_stat].cmp;
				Xr_add = StatTbl[Xr_stat].add;
				Xr_limit = StatTbl[Xr_stat].limit;
			}
		}
	}
}

 void Op::SetNFRQ(int nfrq) {
	if ((Nfrq ^ nfrq) & 0x80) {
//		LfoLevel = CULC_ALPHA;
		LfoLevelReCalc=true;
	}
	Nfrq = nfrq;
	CulcNoiseCycle();
}
 void Op::CulcNoiseCycle() {
	if (Nfrq & 0x80) {
		NoiseCycle = (32-(Nfrq&31)) << 25;
		if (NoiseCycle < NoiseStep) {
			NoiseCycle = NoiseStep;
		}
		NoiseCounter = NoiseCycle;
	} else {
		NoiseCycle = 0;
	}
}


 void Op::Output0(int lfopitch, int lfolevel) {
        int o;
	short Sin;
	
	if (LfoPitch != lfopitch) {
//		DeltaT = ((STEPTBL[Pitch+lfopitch]+Dt1Pitch)*Mul)>>1;
		DeltaT = ((STEPTBL[Pitch+lfopitch]+Dt1Pitch)*Mul)>>(6+1);
		LfoPitch = lfopitch;
	}
	T += DeltaT;
	Sin=(SINTBL[(((T+Out2Fb)>>PRECISION_BITS))&(SIZESINTBL-1)]);

	if(Xr_stat!=RELEASE_MAX){
		int lfolevelame = lfolevel & Ame;
		if ((LfoLevel != lfolevelame || LfoLevelReCalc)&& IS_ZERO_CLOSS(SinBf,Sin)) {
			Alpha = (int)(ALPHATBL[ALPHAZERO+Tl-Xr_el-lfolevelame]);
			LfoLevel = lfolevelame;
			LfoLevelReCalc=false;
		}
		o = (Alpha)
			* (int)Sin ;
		SinBf=Sin;
	}else{o=0;}

//	int o2 = (o+Inp_last) >> 1;
//	Out2Fb = (o+o) >> Fl;
	Out2Fb = (o + Inp_last) >> Fl;
	Inp_last = o;

	*out = o;
	*out2 = o;	// alg=5用
	*out3 = o; // alg=5用
//	*out = o2;
//	*out2 = o2;	// alg=5用
//	*out3 = o2; // alg=5用
};

 void Op::Output(int lfopitch, int lfolevel) {
	int o;
	short Sin;
	
	if (LfoPitch != lfopitch) {
//		DeltaT = ((STEPTBL[Pitch+lfopitch]+Dt1Pitch)*Mul)>>1;
		DeltaT = ((STEPTBL[Pitch+lfopitch]+Dt1Pitch)*Mul)>>(6+1);
		LfoPitch = lfopitch;
	}
	T += DeltaT;
	Sin=(SINTBL[(((T+inp)>>PRECISION_BITS))&(SIZESINTBL-1)]);

	if(Xr_stat!=RELEASE_MAX){
		int lfolevelame = lfolevel & Ame;
	
		if ((LfoLevel != lfolevelame || LfoLevelReCalc)&& IS_ZERO_CLOSS(SinBf,Sin)) {
			Alpha = (int)(ALPHATBL[ALPHAZERO+Tl-Xr_el-lfolevelame]);
			LfoLevel = lfolevelame;
			LfoLevelReCalc=false;
		}
		o = (Alpha)
			* (int) Sin;
		SinBf=Sin;
	}else{o=0;}
	*out += o;
};

 void Op::Output32(int lfopitch, int lfolevel) {
	short Sin;
	if (LfoPitch != lfopitch) {
//		DeltaT = ((STEPTBL[Pitch+lfopitch]+Dt1Pitch)*Mul)>>1;
		DeltaT = ((STEPTBL[Pitch+lfopitch]+Dt1Pitch)*Mul)>>(6+1);
		LfoPitch = lfopitch;
	}
	T += DeltaT;

	int o;
	Sin=(SINTBL[(((T+inp)>>PRECISION_BITS))&(SIZESINTBL-1)]);
	if(Xr_stat!=RELEASE_MAX){
		if (NoiseCycle == 0) {
			int lfolevelame = lfolevel & Ame;
			if ((LfoLevel != lfolevelame || LfoLevelReCalc)&& IS_ZERO_CLOSS(SinBf,Sin)) {
				Alpha = (int)(ALPHATBL[ALPHAZERO+Tl-Xr_el-lfolevelame]);
				LfoLevel = lfolevelame;
				LfoLevelReCalc=false;
			}
			o = (Alpha)
				* (int) Sin;
			SinBf=Sin;
		} else {
			NoiseCounter -= NoiseStep;
			if (NoiseCounter <= 0) {
				NoiseValue = (int)((irnd()>>30)&2)-1;
				NoiseCounter += NoiseCycle;
			}

			int lfolevelame = lfolevel & Ame;
			if (LfoLevel != lfolevelame || LfoLevelReCalc) {
				Alpha = (int)(NOISEALPHATBL[ALPHAZERO+Tl-Xr_el-lfolevelame]);
				LfoLevel = lfolevelame;
				LfoLevelReCalc=false;
			}
			o = (Alpha)
				* NoiseValue * MAXSINVAL;
		}
	}else{o=0;}
	*out += o;
//2002.5.3 test
	*out>>=7;
};


