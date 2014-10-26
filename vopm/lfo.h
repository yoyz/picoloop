#define N_CH 8
#define	SIZELFOTBL	512				// 2^9
#define	SIZELFOTBL_BITS	9
#define	LFOPRECISION	4096	// 2^12

#ifndef __CLASSLFO__
class Lfo {
private:
	int Pmsmul[N_CH];	// 0, 1, 2, 4, 8, 16, 32, 32
	int Pmsshl[N_CH];	// 0, 0, 0, 0, 0,  0,  1,  2
	int	Ams[N_CH];	// 左シフト回数 31(0), 0(1), 1(2), 2(3)
	int	PmdPmsmul[N_CH];	// Pmd*Pmsmul[]
	int	Pmd;
	int	Amd;

	int LfoStartingFlag;	// 0:LFO停止中  1:LFO動作中
	int	LfoOverFlow;	// LFO tのオーバーフロー値
	int	LfoTime;	// LFO専用 t
	int	LfoTimeAdd;	// LFO専用Δt
	int LfoIdx;	// LFOテーブルへのインデックス値
	int LfoSmallCounter;	// LFO周期微調整カウンタ (0～15の値をとる)
	int LfoSmallCounterStep;	// LFO周期微調整カウンタ用ステップ値 (16～31)
	int	Lfrq;		// LFO周波数設定値 LFRQ
	int	LfoWaveForm;	// LFO wave form

	int	PmTblValue,AmTblValue;
	int	PmValue[N_CH],AmValue[N_CH];

	char	PmTbl0[SIZELFOTBL], PmTbl2[SIZELFOTBL];
	unsigned char	AmTbl0[SIZELFOTBL], AmTbl2[SIZELFOTBL];

	 void CulcTblValue();
	 void	CulcPmValue(int ch);
	 void	CulcAmValue(int ch);
	 void CulcAllPmValue();
	 void CulcAllAmValue();

public:
	Lfo(void);
	~Lfo() {};

	 void Init();
	 void InitSamprate();

	 void LfoReset();
	 void LfoStart();
	 void SetLFRQ(int n);
	 void SetPMDAMD(int n);
	 void	SetWaveForm(int n);
	 void	SetPMSAMS(int ch, int n);

	 void	Update();
	 int	GetPmValue(int ch);
	 int	GetAmValue(int ch);
};
#define __CLASSLFO__
#endif
