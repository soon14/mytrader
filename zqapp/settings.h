#pragma once

#include "utility.h"
#include <view.h>

class SkinInfo : public zqdb::SkinInfo
{
public:

	//���
	
	//��ɫ
	std::shared_ptr<wxRibbonDefaultArtProvider> artProvider;
	wxColour crPrimary;			//��ɫ��������ɫ����
	wxColour crSecondary;		//��ɫ��������ɫ����
	wxColour crTertiary;		//����ɫ��������ɫ����

	wxColour crViewBkgnd;		//��ͼɫ���������ڣ�
	wxColour crViewForgnd;		//��ͼ����
	wxColour crCtrlBkgnd;		//�ؼ�ɫ���������ڣ�
	wxColour crCtrlForgnd;		//��ͼ����
	
	//����

	//��ˢ

	//����

	//����߿�

	//����߿������ˮƽ���ߴ�ֱ���֣�
};

template<class T>
class SettingsMap 
	: public boost::property_tree::ptree
	, public zqdb::SkinMap<T,SkinInfo>
{
	typedef zqdb::SkinMap<T, SkinInfo> SkinBase;
public:

	void Clear()
	{
		SkinBase::Clear();
	}
	inline bool IsDispOk() const {
		return SkinBase::IsDispOk();
	}
};
