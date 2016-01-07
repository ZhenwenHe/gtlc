/*
* Geosciences Template Library
*
* Copyright (c) 2008
* Zhenwen He (zwhe@hotmail.com)
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Zhenwen He  makes no  representations
* about the suitability of this software for any purpose. 
* It is provided "as is" without express or implied warranty.
*/
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <list>
#include <map>
#include "lock.h"
#include "feature.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace


/** @defgroup TemporarySets TempSet-��ʱҪ�ؼ��ϣ����ڴ���м�������  
*  @{
*/
class CORE_API TempSet
{
	/** @name ������Ա����˵��  
	*  @{
	*/
protected:
	typedef std::pair< FCLSID,FeatureVectorSharedPtr>   FSPAIR;
	typedef std::map< FCLSID,FeatureVectorSharedPtr >   FSMAP;
	
	/**Ҫ���༯�б�*/
	FSMAP  _set;
	/**��ʱ���ݼ���״̬*/
	int _status;
	/**��ʱ���ݼ�������*/
	std::string    _Name;
	/**Ҫ���༯�б�*/
	int _drawMode;
	/**��ʱ���ݼ�����ɫ*/
	int _color;
	/**��ʱ���ݼ���͸����*/
	float _alpha;
protected:
	/**�������캯������ֹ���洴��*/
	TempSet();

	/** @} */ 

public:
	/** @name �������� 
	*  @{
	*/
	virtual ~TempSet();
	/** @} */ 

	/** ��Ԫ�� */
	friend class TemporarySets;
	/** @} */ 

	/** @name ����������  
	*  @{
	*/
	/**  ������ʱ���ݼ��Ļ���ģʽ
	*	 @param [in]      mode  ��ʱ���ݼ��Ļ���ģʽ
	*   @return ��    
	*/
	inline void setDrawMode(const int &mode){_drawMode = mode;}

	/**  ��ȡ��ʱ���ݼ��Ļ���ģʽ
	*	 @param [in]      ��  
	*    @return ��ʱ���ݼ��Ļ���ģʽ  
	*/
	inline int getDrawMode(){return _drawMode;}

	/**  ������ʱ���ݼ�����ɫ
	*	 @param [in]      color  ��ʱ���ݼ�����ɫ
	*   @return ��    
	*/
	inline void setColor(const int &color){_color = color;}

	/**  ��ȡ��ʱ���ݼ�����ɫ
	*	 @param [in]      ��  
	*    @return ��ʱ���ݼ�����ɫ   
	*/
	inline int getColor(){return _color;}

	/**  ������ʱ���ݼ���͸����
	*	 @param [in] alpha  ��ʱ���ݼ���͸����
	*   @return ��    
	*/
	inline void setAlpha(const float &alpha){_alpha = alpha;}

	/**  ��ȡ��ʱ���ݼ���͸����
	*	 @param [in]      ��  
	*    @return ��ʱ���ݼ���͸����
	*/
	inline float getAlpha(){return _alpha;}

	/**  ������ʱ���ݼ������ƣ���һ��TemporarySets��ʽΨһ��
	*	 @param [in]      strName  ��ʱ���ݼ�������
	*   @return ��    
	*/
	inline void setName(std::string strName){_Name = strName;}

	/**  ��ȡ��ʱ���ݼ������ƣ���һ��TemporarySets��ʽΨһ��
	*	 @param [in]      ��  
	*    @return ��ʱ���ݼ�������    
	*/
	inline std::string getName(){ return _Name;}

	/**  ������ʱ���ݼ���״̬
	*	 @param [in] status  ��ʱ���ݼ���״̬
	*    @return ��    
	*/
	inline void setStatus(int status){ _status = status;}

	/**  ��ȡ��ʱ���ݼ���״̬
	*	 @param ��  
	*    @return ��ʱ���ݼ���״̬    
	*/
	inline int getStatus(){ return _status;}


	/**  ��ȡָ��Ҫ�����͵�����Ҫ��,���ص�ָ�벻���ⲿ����
	*	 @param [in] fclsid Ҫ�����͵�ΨһID  
	*    @return ��ʱ���ݼ���Ҫ������Ϊfclsid��Feature����    
	*/
	FeatureVectorSharedPtr getFeatures(FCLSID fclsid);

	/**  �Ƴ�ָ��Ҫ�����͵�����Ҫ��
	*	 @param [in] fclsid Ҫ�����͵�ΨһID  
	*    @return ��  
	*/
	FeatureVectorSharedPtr removeFeatures(const FCLSID &fclsid);



	/**  �����ݼ���ͨ��FCLSIDȷ��Ӧ�ò����ĸ����������У�������
	*	 @param [in] fsp Ҫ������ʱ���ݼ��ϵ�Feature�Ĺ���ָ��  
	*    @return ��    
	*/
	void addFeature(FeatureSharedPtr fsp);
	/**  �����ݼ���ͨ��FCLSID��FIDȷ��Ӧ���Ƴ��ĸ�Ҫ�ض��󣬲�����ʱ�������Ƴ���
	*	 @param [in] fclsid Ҫ������ID
	*	 @param [in] fid Ҫ��ID
	*    @return ��    
	*/ 
	FeatureSharedPtr removeFeature(const FCLSID &fclsid,const FID &fid);

	/**  ��ȡ���ݼ����е�����Fclsid
	*	 @param [in] fclsids   �����洢����FCLSID��vector����
	*    @return ��    
	*/
	void getAllFclsid(vector<FCLSID> &fclsids);
	/**  ���Map
	*	 @param ��  
	*    @return ��    
	*/
	void clear( );

	/** @} */ 

};
typedef std::shared_ptr<TempSet> TempSetSharedPtr;
/** @} */ 


/** @addtogroup TemporarySets    TemporarySets
*  @{
*/
class CORE_API TemporarySets
{
	/** @name ������Ա����˵��  
	*  @{
	*/
protected:
	/**��ʱ���ݼ����б�*/
	std::list<TempSetSharedPtr>  _data;

	/** @} */  
public:
	/** @name ����������������
	*  @{
	*/
	TemporarySets(void);
	virtual ~TemporarySets(void);
	/** @} */  
public:
	/** @name ������������  
	*  @{
	*/
	/**  �ڼ����д���һ��ָ����������ʱ���ݼ���������Ψһ��ʶ����������ɹ��򷵻�ָ�룬���򷵻ؿ�ָ��
	*	 @param [in]  strSetName Ҫ��������ʱ���ݼ�������  
	*    @return ��������ɹ��������TemporarySets�У�����ָ�룬���򷵻ؿ�ָ�룻ע�ⷵ�ص�ָ�벻��Ҫ�ⲿ��������    
	*/
	TempSetSharedPtr newTempSet(const std::string &strSetName);
	/**  �Ӽ����в���һ��ָ����������ʱ���ݼ���������Ψһ��ʶ��������ҵ��򷵻�ָ�룬���򷵻ؿ�ָ��
	*	 @param [in]  strSetName ��ʱ���ݼ�������  
	*    @return ����ɹ�����ָ�룬���򷵻ؿ�ָ�룻ע�ⷵ�ص�ָ�벻��Ҫ�ⲿ��������    
	*/
	TempSetSharedPtr findTempSet(const std::string &strSetName);
	/**  �Ӽ����в��ҵ�i��tempset
	*	 @param [in]    �±�i
	*    @return ����ɹ�����ָ�룬���򷵻ؿ�ָ�룻ע�ⷵ�ص�ָ�벻��Ҫ�ⲿ��������    
	*/
	TempSetSharedPtr getTempSetAt(const size_t &i);
	/**  �Ӽ����л�ȡ��i��tempset
	*	 @param [in]    �±�i
	*    @return ����ɹ�����ָ�룬���򷵻ؿ�ָ�룻ע�ⷵ�ص�ָ�벻��Ҫ�ⲿ��������    
	*/
	TempSetSharedPtr operator () (size_t i);
	/**  �Ӽ�����ɾ����i��tempset
	*	 @param [in]    strSetName ��ʱ���ݼ�������
	*    @return ����ɹ�����ָ�룬���򷵻ؿ�ָ�룻ע�ⷵ�ص�ָ�벻��Ҫ�ⲿ��������    
	*/
	TempSetSharedPtr removeTempSet(const std::string &strSetName);
	/**  ��ȡ�������ʱ������
	*	 @param ��
	*    @return ���ع������ʱ������ 
	*/
	inline size_t size(){	return _data.size();}
	/**  ���TemporarySets���ú��������������Զ����ã�Ҳ�������û���ε���
	*	 @param [in]      ��  
	*    @return ��    
	*/
	void clear();
	/** @} */ 

};

/** @} */ 

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
