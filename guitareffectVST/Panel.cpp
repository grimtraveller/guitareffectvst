#include "Panel.h"
#include <Windows.h>
#include <Commdlg.h>
namespace PanelNS
{

void Panel::LoadPreset(int prevPresetIdx)
{
	// 그냥 이펙트의 속성을 얻어서 파라메터로 저장한다.

	Button *but = (Button *)mGUI->GetElement(mPresetButton);
		if(preset < mPresetStrs.size())
			but->mLabel = mPresetStrs[prevPresetIdx];
	::Slider *slider;
	OnOffButton *onoff;
	if(mDoICallSetPreset) mEffect->setpreset(preset);

	for(int i=0; i<mData.size();++i)
	{
		int par = 0;
		par = mEffect->getpar(mData[i].parIdx);
		if(mData[i].type == Slider)
		{
			slider = (::Slider*)(mGUI->GetElement(mData[i].handle));
			if(mData[i].isFreq)
				slider->SetVal(RealToPrint(i, GetRealMinMaxByFreq(par)));
			else
				slider->SetVal(RealToPrint(i, par));
		}
		else if(mData[i].type == OnOff)
		{
			onoff = (OnOffButton*)mGUI->GetElement(mData[i].handle);
			if(par)
			{
				onoff->mOn = true;
				mCBOnOffs[mData[i].cbIdx]->OnOn();
			}
			else
			{
				onoff->mOn = false;
				mCBOnOffs[mData[i].cbIdx]->OnOff();
			}
		}
		else if(mData[i].type == Selection)
		{
			mCBLists[mData[i].cbListIdx]->OnSelect(par);
		}
	}


}
void Panel::SetPreset(int preset)
{
	// WahWah는 setpreset에 약간 매직이 있다. 그러므로 WahWah인 경우엔 따로함.
	// EQ2는 다른 이펙트랑 별로 다르지 않을 것 같은데? EQ도 이걸로 할 수 있다. parIdx를 잘 해주면 됨
	// Flange는 Chorus랑 똑같으니 그냥 같은걸 쓰자
	// 다른 예외상황을 가면서 찾아보자.
	Button *but = (Button *)mGUI->GetElement(mPresetButton);
	mPrevPreset = preset;
	but->mLabel = mPresetStrs[preset];
	::Slider *slider;
	OnOffButton *onoff;
	for(int i=0; i<mData.size();++i)
	{

		if(mData[i].type == Slider && mUsePresetIdx)
		{
			slider = (::Slider*)(mGUI->GetElement(mData[i].handle));
			if(mData[i].isFreq)
				slider->SetVal(RealToPrint(i, GetRealMinMaxByFreq(mPresets[preset][mData[i].preIdx])));
			else
				slider->SetVal(RealToPrint(i, mPresets[preset][mData[i].preIdx]));
		}
		else if(mData[i].type == Slider && mData[i].parIdx < mSizePreset)
		{
			slider = (::Slider*)(mGUI->GetElement(mData[i].handle));
			if(mData[i].isFreq)
				slider->SetVal(RealToPrint(i, GetRealMinMaxByFreq(mPresets[preset][mData[i].parIdx])));
			else
				slider->SetVal(RealToPrint(i, mPresets[preset][mData[i].parIdx]));
		}
		else if(mData[i].type == OnOff && mData[i].parIdx < mSizePreset && mUsePresetIdx)
		{
			onoff = (OnOffButton*)mGUI->GetElement(mData[i].handle);
			if(mPresets[preset][mData[i].preIdx])
			{
				onoff->mOn = true;
				mCBOnOffs[mData[i].cbIdx]->OnOn();
			}
			else
			{
				onoff->mOn = false;
				mCBOnOffs[mData[i].cbIdx]->OnOff();
			}
		}
		else if(mData[i].type == Selection && mData[i].parIdx < mSizePreset && mUsePresetIdx)
		{
			mCBLists[mData[i].cbListIdx]->OnSelect(mPresets[preset][mData[i].preIdx]);
		}
		else if(mData[i].type == OnOff && mData[i].parIdx < mSizePreset)
		{
			onoff = (OnOffButton*)mGUI->GetElement(mData[i].handle);
			if(mPresets[preset][mData[i].parIdx])
			{
				onoff->mOn = true;
				mCBOnOffs[mData[i].cbIdx]->OnOn();
			}
			else
			{
				onoff->mOn = false;
				mCBOnOffs[mData[i].cbIdx]->OnOff();
			}
		}
		else if(mData[i].type == Selection && mData[i].parIdx < mSizePreset)
		{
			mCBLists[mData[i].cbListIdx]->OnSelect(mPresets[preset][mData[i].parIdx]);
		}
	}
	
	if(mDoICallSetPreset) mEffect->setpreset(preset);
}
void Panel::DrawText()
{
	int x,y,w=180,h=300;
	if(mWhereis < 5)
	{
		x = 180*mWhereis;
		y = 150+35;
	}
	else
	{
		x = 180*(mWhereis-5);
		y = 150+300+35;
	}
	TextOption op2(x,y-35+3,180,10, 0,0,0,255);
	mGUI->Print2(op2, mEffName.c_str());
	mGUI->Print(TextOption(x+45,y-35+13,60, 20, 0,0,0,255), "Preset:");

	for(int i=0; i<mData.size(); ++i)
	{
		if(mData[i].type == Slider)
		{
			mGUI->Print(TextOption(x,y,60, 13, 0,0,0,255), mData[i].text.c_str());
			y += 13;
		}
		else if(mData[i].type == Selection)
		{
			mGUI->Print(TextOption(x,y,60, 18, 0,0,0,255), mData[i].text.c_str());
			y += 18;
		}
		else //on off
		{
			if(!mData[i].useSameYAsPrev)
			{
				y += 18;
			}
		}
	}

}
Panel::Panel(GLGUI *gui, VstPlugin *plug, Effect *effect, string effName, int whereis, int *presets, int sizePreset, int numPresets, vector<string> presetTexts, bool usePresetIdx, bool doICallSetPreset)
	:mGUI(gui), mWhereis(whereis), mPlug(plug), mEffect(effect), mEffName(effName), mPresetStrs(presetTexts), mSizePreset(sizePreset), mUsePresetIdx(usePresetIdx), mDoICallSetPreset(doICallSetPreset)
{
	mPrevPreset = 0;
	for(int y=0; y<numPresets; ++y)
	{
		vector<int> newPreset;
		
		for(int x=0; x<sizePreset; ++x)
		{
			newPreset.push_back(presets[y*sizePreset + x]);
		}
		mPresets.push_back(newPreset);
	}
	mY = 0;


	int x,y,w=180,h=300;
	if(mWhereis < 5)
	{
		x = 180*mWhereis;
		y = 150+35;
	}
	else
	{
		x = 180*(mWhereis-5);
		y = 150+300+35;
	}

	cbPresetSelect = new PresetCallback(this);
	cbPresetSelected = new PresetListCallback(this);
	mButtons.push_back(mGUI->NewButton(x+100,y-35+13,80, 20, "Default", cbPresetSelect)); // Preset
	mPresetButton = *(mButtons.end()-1);

}
Panel::~Panel()
{
	for(vector<int>::iterator it = mButtons.begin(); it != mButtons.end(); ++it)
	{
		mGUI->DeleteGUIElement(*it);
	}
	for(vector<PanelSliderCallback*>::iterator it=mCBSliders.begin(); it != mCBSliders.end(); ++it)
	{
		delete *it;
	}
	for(vector<PanelButtonCallback*>::iterator it=mCBButtons.begin(); it != mCBButtons.end(); ++it)
	{
		delete *it;
	}
	for(vector<PanelOnOffCallback*>::iterator it=mCBOnOffs.begin(); it != mCBOnOffs.end(); ++it)
	{
		delete *it;
	}
	for(vector<PanelListCallback*>::iterator it=mCBLists.begin(); it != mCBLists.end(); ++it)
	{
		delete *it;
	}
	delete cbPresetSelect;
	delete cbPresetSelected;
}
void Panel::AddParamData(Data &data, bool unused)
{
	mData.push_back(data);
	if(unused) return;

	int x,y,w=180,h=300;
	if(mWhereis < 5)
	{
		x = 180*mWhereis;
		y = 150+35;
	}
	else
	{
		x = 180*(mWhereis-5);
		y = 150+300+35;
	}

	if(data.type == Slider)
	{
		mCBSliders.push_back(new PanelSliderCallback(this, mData.size()-1));
		mData[mData.size()-1].cbIdx = mCBSliders.size()-1;
		mData[mData.size()-1].handle = mGUI->NewSlider(x+60,y+mY,120, mData[mData.size()-1].valPrintMin, mData[mData.size()-1].valPrintMax);
		mGUI->SetSliderCallback(mData[mData.size()-1].handle, mCBSliders[mCBSliders.size()-1]);
		mY += 13;
	}
	else if(data.type == OnOff)
	{
		mCBOnOffs.push_back(new PanelOnOffCallback(this, mData.size()-1));
		mData[mData.size()-1].cbIdx = mCBOnOffs.size()-1;
		if(!mData[mData.size()-1].useSameYAsPrev)
		{
			mData[mData.size()-1].handle = mGUI->NewOnOffButton(x+5,y+mY,180/2-10, 18, mData[mData.size()-1].text, mCBOnOffs[mCBOnOffs.size()-1]);
			mY += 18;
		}
		else
		{
			mData[mData.size()-1].handle = mGUI->NewOnOffButton(x+180/2+5,y+mY-18,180/2-10, 18, mData[mData.size()-1].text, mCBOnOffs[mCBOnOffs.size()-1]);
		}
		
	}
	else if(data.type == Selection)
	{
		mCBLists.push_back(new PanelListCallback(this, mData.size()-1));
		mCBButtons.push_back(new PanelButtonCallback(this, mData.size()-1, mCBLists[mCBLists.size()-1]));
		mData[mData.size()-1].cbIdx = mCBButtons.size()-1;
		mData[mData.size()-1].cbListIdx = mCBLists.size()-1;
		mTypeStrs.push_back(mData[mData.size()-1].mTypeStrs);
		mData[mData.size()-1].typeStrsIdx = mTypeStrs.size()-1;
		mData[mData.size()-1].handle = mGUI->NewButton(x+60,y+mY,180-60, 18, "None", mCBButtons[mCBButtons.size()-1]);
		mY += 18;
	}
	else if(data.type == Browser)
	{
		mCBBrowser = new PanelBrowseCallback(this, mData.size()-1);
		mY += 18;
		mData[mData.size()-1].handle = mGUI->NewButton(x+60,y+mY,180-60, 18, "Browse", mCBBrowser);
	}
	mButtons.push_back(mData[mData.size()-1].handle); // Preset
	
}


PanelSliderCallback::PanelSliderCallback(Panel *a, int dataIdx)
	: mPanel(a), mDataIdx(dataIdx)
{
}
void PanelSliderCallback::SetVal(int val)
{
	int newval = mPanel->PrintToReal(mDataIdx, val);
	if(mPanel->mData[mDataIdx].isFreq)
	{
		mPanel->mEffect->changepar(mPanel->mData[mDataIdx].parIdx, GetFreqByRealMinMax(newval));
	}
	else
	{
		mPanel->mEffect->changepar(mPanel->mData[mDataIdx].parIdx, newval);
	}
	
}
PanelListCallback::PanelListCallback(Panel *a, int dataIdx)
	: mPanel(a), mDataIdx(dataIdx)
{
}
void PanelListCallback::OnSelect(int idx)
{
	if(idx != -1 && idx <= mPanel->mData[mDataIdx].valRealMax)
	{
		mPanel->mGUI->mPopupList->hidden = true;
		mPanel->mGUI->mPopupList->Clear();
		mPanel->mEffect->changepar(mPanel->mData[mDataIdx].parIdx, idx);
		Button *but = (Button *)mPanel->mGUI->GetElement(mPanel->mData[mDataIdx].handle);
		but->mLabel = mPanel->mTypeStrs[mPanel->mData[mDataIdx].typeStrsIdx][idx];
	}
	else
	{
		char temp[123];
		sprintf(temp, "%d %d %d", idx, mDataIdx, mPanel->mData[mDataIdx].parIdx);
		//MessageBox(NULL, temp, temp, MB_OK);
	}
}
void PanelListCallback::OnPageSelect(int idx)
{


}
PanelOnOffCallback::PanelOnOffCallback(Panel *a, int dataIdx)
	: mPanel(a), mDataIdx(dataIdx)
{
}
void PanelOnOffCallback::OnOn()
{
	mPanel->mEffect->changepar(mPanel->mData[mDataIdx].parIdx, 1);
}
void PanelOnOffCallback::OnOff()
{
	mPanel->mEffect->changepar(mPanel->mData[mDataIdx].parIdx, 0);
}
PanelButtonCallback::PanelButtonCallback(Panel *a, int dataIdx, PanelListCallback *cbList) // dataIdx만 있으면 parIdx등을 다 얻어올 수 있다.
	: mPanel(a), mDataIdx(dataIdx), mCBList(cbList)
{
}
void PanelButtonCallback::OnClick()
{
	mPanel->mGUI->mPopupList->hidden = false;
	mPanel->mGUI->mPopupList->Clear();

	for(int i=0;i<mPanel->mTypeStrs[mPanel->mData[mDataIdx].typeStrsIdx].size();++i)
	{
		mPanel->mGUI->mPopupList->Add(mPanel->mTypeStrs[mPanel->mData[mDataIdx].typeStrsIdx][i]);
	}
	mPanel->mGUI->mPopupList->SetCallback(mCBList);
}
PresetCallback::	PresetCallback(Panel *a)
	:mPanel(a), ButtonCallback()
{
}
void PresetCallback::OnClick()
{
	mPanel->mGUI->mPopupList->hidden = false;
	mPanel->mGUI->mPopupList->Clear();

	for(int i=0;i<mPanel->mPresets.size();++i)
	{
		mPanel->mGUI->mPopupList->Add(mPanel->mPresetStrs[i]);
	}
	mPanel->mGUI->mPopupList->SetCallback(mPanel->cbPresetSelected);
	
}
PresetListCallback::PresetListCallback(Panel *a):mPanel(a), TabbedListBoxCallback()
{
}
void PresetListCallback::OnPageSelect(int idx)
{
}
void PresetListCallback::OnSelect(int idx)
{
	if(idx != -1)
	{
		mPanel->mGUI->mPopupList->hidden = true;
		mPanel->mGUI->mPopupList->Clear();
		mPanel->SetPreset(idx);
	}

}
PanelBrowseCallback::PanelBrowseCallback(Panel *a, int dataIdx) // dataIdx만 있으면 parIdx등을 다 얻어올 수 있다.
	: mPanel(a), mDataIdx(dataIdx)
{
}
void PanelBrowseCallback::OnClick()
{
	OPENFILENAME ofn ;
// a another memory buffer to contain the file name
	char szFile[260] ;
	
    // open a file name
	ZeroMemory( &ofn , sizeof( ofn));
	ofn.lStructSize = sizeof ( ofn );
	ofn.hwndOwner = NULL  ;
	ofn.lpstrFile = szFile ;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof( szFile );
	char buffer[260];
	if((Echotron*)mPanel->mEffect == mPanel->mPlug->mEffEchotron)
	{
		ofn.lpstrFilter = "Echotron File\0*.dly\0";
		GetModuleFileName((HMODULE)mPanel->mPlug->mHInstance, buffer, sizeof(buffer));
		string a(buffer);
		int pos = a.find_last_of('\\');
		string b = a.substr(0, pos);
		b += "\\guitareffectVST\\data";
		strcpy(buffer, b.c_str());
		ofn.lpstrInitialDir=buffer;
	}
	else if((Reverbtron*)mPanel->mEffect == mPanel->mPlug->mEffReverbtron)
	{
		ofn.lpstrFilter = "Reverbtron File\0*.rvb\0";
		GetModuleFileName((HMODULE)mPanel->mPlug->mHInstance, buffer, sizeof(buffer));
		string a(buffer);
		int pos = a.find_last_of('\\');
		string b = a.substr(0, pos);
		b += "\\guitareffectVST\\data";
		strcpy(buffer, b.c_str());
		ofn.lpstrInitialDir=buffer;
	}
	else if((Convolotron*)mPanel->mEffect == mPanel->mPlug->mEffConvolotron)
	{
		ofn.lpstrFilter = "Convolotron File\0*.wav\0";
		GetModuleFileName((HMODULE)mPanel->mPlug->mHInstance, buffer, sizeof(buffer));
		string a(buffer);
		int pos = a.find_last_of('\\');
		string b = a.substr(0, pos);
		b += "\\guitareffectVST\\data\\IR";
		strcpy(buffer, b.c_str());
		ofn.lpstrInitialDir=buffer;
	}
	
	ofn.nFilterIndex =1;
	ofn.lpstrFileTitle = NULL ;
	ofn.nMaxFileTitle = 0 ;

	ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;
	GetOpenFileName( &ofn );
	

	if((Echotron*)mPanel->mEffect == mPanel->mPlug->mEffEchotron)
	{
		strcpy(mPanel->mPlug->mEffEchotron->Filename, ofn.lpstrFile);
		mPanel->mPlug->mEffEchotron->setfile(0);
	}
	else if((Reverbtron*)mPanel->mEffect == mPanel->mPlug->mEffReverbtron)
	{
		strcpy(mPanel->mPlug->mEffReverbtron->Filename, ofn.lpstrFile);
		mPanel->mPlug->mEffReverbtron->setfile(0);
	}
	else if((Convolotron*)mPanel->mEffect == mPanel->mPlug->mEffConvolotron)
	{
		strcpy(mPanel->mPlug->mEffConvolotron->Filename, ofn.lpstrFile);
		mPanel->mPlug->mEffConvolotron->setfile(0);
	}
	// Now simpley display the file name 
	//MessageBox ( NULL ,  , "Select file" , MB_OK);

	/*
	mPanel->mGUI->mPopupList->hidden = false;
	mPanel->mGUI->mPopupList->Clear();

	for(int i=0;i<mPanel->mTypeStrs[mPanel->mData[mDataIdx].typeStrsIdx].size();++i)
	{
		mPanel->mGUI->mPopupList->Add(mPanel->mTypeStrs[mPanel->mData[mDataIdx].typeStrsIdx][i]);
	}
	mPanel->mGUI->mPopupList->SetCallback(mCBList);
	*/
}

/*
#include "stdafx.h"
#include <windows.h>
#include "resource.h"
#include <Commdlg.h>
// 
// Gobal Variables and declarations.
// 
OPENFILENAME ofn ;
// a another memory buffer to contain the file name
char szFile[100] ;
int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow )
{
	
    // open a file name
	ZeroMemory( &ofn , sizeof( ofn));
	ofn.lStructSize = sizeof ( ofn );
	ofn.hwndOwner = NULL  ;
	ofn.lpstrFile = szFile ;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof( szFile );
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex =1;
	ofn.lpstrFileTitle = NULL ;
	ofn.nMaxFileTitle = 0 ;
	ofn.lpstrInitialDir=NULL ;
	ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;
	GetOpenFileName( &ofn );
	
	// Now simpley display the file name 
	MessageBox ( NULL , ofn.lpstrFile , "File Name" , MB_OK);
	return 0;
}
}
*/


};
