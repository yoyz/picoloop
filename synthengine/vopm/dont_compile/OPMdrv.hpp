#ifndef __OPMDRV__
#include <list>
//#include "aeffectx.h"
using namespace std;

#define CHMAX 8
#define VOMAX 128

#define ERR_NOERR 0x0000
#define ERR_NOFILE 0x10000000
#define ERR_SYNTAX 0x20000000
#define ERR_LESPRM 0x30000000

#define CHSLOT8 15 //Midi 15Ch assigned OPM8ch

class OPDATA{
	public:
	 //EG-Level
		unsigned int TL;
		unsigned int D1L;
	 //EG-ADR
		unsigned int AR;
		unsigned int D1R;
		unsigned int D2R;
		unsigned int RR;
	 //DeTune
		unsigned int KS;
		unsigned int DT1;
		unsigned int MUL;
		unsigned int DT2;
		unsigned int F_AME;

		OPDATA(void);
};

class CHDATA{
	public:
		unsigned int F_PAN;//L:0x80Msk R:0x40Msk
		unsigned int CON;
		unsigned int FL;
		unsigned int AMS;
		unsigned int PMS;
		unsigned int F_NE;
		unsigned int OpMsk;
		class OPDATA Op[4];
		char Name[16];
		unsigned int SPD;
		unsigned int PMD;
		unsigned int AMD;
		unsigned int WF;
		unsigned int NFQ;
		CHDATA(void);
// ~CHDATA(void);

};


struct TBLMIDICH{
	int VoNum;				//音色番号 0-
	int Bend;				//ピッチベンド -8192～+8191[signed 14bit]
	int Vol;				//ボリューム 0-127
	int Pan;				//L:0x80 Msk,R:0x40 Msk
	int BendRang;			//ピッチベンドレンジ　1-??[半音の倍率]
	int PMD;
	int AMD;
	int SPD;
	int OpTL[4];			//OpTLの相対変化
	unsigned int RPN;
	unsigned int NRPN;
	bool CuRPN;
	int BfNote;				//前回のノート番号（ポルタメント用）
	int PortaSpeed;			//ポルタメントスピード
	bool PortaOnOff;
	int LFOdelay;
};





class OPMDRV{
// friend class VOPM;
private:
	unsigned int SampleTime;	//HOSTから指定されたDelta時間
	char CuProgName[64];		//現在HOSTに選択されているプログラム名
	int CuProgNum;				//現在HOSTに選択されているプログラム番号

	int NoisePrm;				//OPM ノイズパラメータ
	int MstVol;					//マスターボリューム
	bool PolyMono;				//Poly true,Mono false
	class Opm *pOPM;
	int OpmClock;				//OPMのクロック 0:3.58MHz 1:4MHz
//発音CH管理双方向リスト
	list<int> PlayCh,WaitCh;
	
	struct {
		int VoNum;				//音色番号0-
		int MidiCh;				//Midiチャンネル
		int Note;				//ノート番号(0-127,未使用時255)
		int Vel;				//ベロシティ
		int PortaBend;			//ポルタメント用のベンドオフセット
		int PortaSpeed;
		int LFOdelayCnt;
	}TblCh[CHMAX];

	struct TBLMIDICH TblMidiCh[16];	//Midiチャンネル設定テーブル

	int AtachCh(void);			//チャンネル追加
	void DelCh(int);			//チャンネル削除
	int ChkEnCh(void);			//空きチャンネルを返す。
	int ChkSonCh(int,int);		//MidiChとベロシティより発音中のチャンネルを探す。
	int NonCh(int,int,int);		//MidiChとノート、ベロシティで発音する。
	int NoffCh(int,int);		//MidiChとノートで消音する。

	void SendVo(int);			//音色パラメータをOPMに送る
	void SendKc(int);			//音階データをOPMにセットする
	void SendTl(int);			//チャンネル音量をセットする
	void SendPan(int,int);		//Panをセットする
	void SendLfo(int);


public:
	OPMDRV(class Opm *);				//コンストラクタ

	class CHDATA VoTbl[VOMAX];	//音色テーブル
															//SLOT 1音毎の設定クラス
  //MidiProgramName MidiProg[16];
	void setDelta(unsigned int);						//コマンドDelta時間設定
	void NoteOn(int Ch,int Note,int Vol);		//ノートＯＮ
	void NoteOff(int Ch,int Note);					//ノートＯＦＦ
	void NoteAllOff(int Ch);								//全ノートＯＦＦ
	void ForceAllOff(int Ch);								//強制発音停止
	void ForceAllOff();											//強制発音停止(全チャンネル対応)
	void BendCng(int Ch,int Bend);					//ピッチベンド変化
	void VolCng(int Ch,int Vol);						//ボリューム変化
	void VoCng(int Ch,int VoNum);						//音色チェンジ
	void MsVolCng(int MVol);								//マスターボリューム変化
	void PanCng(int Ch,int Pan);						//Pan変化
	int Load(char *);												//音色、LFO設定ロード
	int Save(char *);												//音色、LFO設定セーブ
	int getProgNum(void);										//音色ナンバー取得
	void setProgNum(long );									//音色ナンバーセット
	void setProgName(char* );								//音色名セット
	char* getProgName(void);								//音色名取得
	char* getProgName(int);									//音色名取得
	unsigned char getPrm(int);							//index(Vst kNum)によるパラメータセット
	void setPrm(int,unsigned char);					//index(Vst kNum)によるパラメータ取得
	void setRPNH(int,int);
	void setNRPNH(int,int);
	void setRPNL(int,int);
	void setNRPNL(int,int);
	void setData(int,int);
	void setPoly(bool);
	void AutoSeq(int sampleframes);
	void setPortaSpeed(int MidiCh,int speed);
	void setPortaOnOff(int MidiCh,bool onoff);
	void setPortaCtr(int MidiCh,int Note);
	void setLFOdelay(int MidiCh,int data);

	~OPMDRV(void);									//デストラクタ
  //long getMidiProgram(long channel,MidiProgramName *curProg);
	void ChDelay(int );


	void setCcTL_L(int MidiCh,int data,int OpNum);
	void setCcTL_H(int MidiCh,int data,int OpNum);

	void setCcLFQ_L(int MidiCh,int data);
	void setCcLFQ_H(int MidiCh,int data);

	void setCcPMD_L(int MidiCh,int data);
	void setCcPMD_H(int MidiCh,int data);

	void setCcAMD_L(int MidiCh,int data);
	void setCcAMD_H(int MidiCh,int data);

private:
	int CalcLim8(int VoData,int DifData);
	int CalcLim7(int VoData,int DifData);
	int CalcLimTL(int VoData,int DifData);

	void setAMD(int ch,int Amd);
	void setPMD(int ch,int Pmd);
	void setSPD(int ch,int Spd);
	void setTL(int ch,int Tl,int OpNum);
};
#define __OPMDRV__
#endif
