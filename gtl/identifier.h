#pragma once
#include "buffer.h"
begin_gtl_namespace 

template<
	int BITS =64,
	typename ELEMENT=char,
	typename TRAITS=std::char_traits<ELEMENT>,
	typename ALLOC=std::allocator<char> >
class  Identifier 
	: Object<ELEMENT,TRAITS,ALLOC>
{
public:
	typedef uint64_t raw_type;
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "Identifier";
	}
		
	virtual void writeBuffer(void*  & buffer, size_t & count){
		count = BITS/8;
		if(buffer==0)
			buffer = new uint8_t[count];
		memcpy(buffer,_bid,count);
	}

	virtual void readBuffer(const void *  buffer, size_t count=0){		
		memcpy(_bid,buffer, BITS/8);
	}
	virtual Object * clone() {
		return (Object *) new Identifier(*this);
	}
public:
	 
	union ULONGNumber
	{
		uint32_t ns;
		uint8_t bs[BITS/8];
	};
	 
	union {
		struct 	{			
			/** 用户ID区域 */
			uint8_t user_id;
			/** 自动增加区域 */
			uint8_t numbers[BITS/8-1];
			/** ID生成时间 */
			uint32_t time_stamp;
		}_sid;
		/** 64位原始值 */
		uint64_t  _id;
		/** 以BYTE表示的64位值 */
		uint8_t _bid[BITS/8]; 
	} ;
	 
 
public:
	/** @name 构造与析构函数集  
	*  @{
	*/
	/** 采用随机用户ID生成Identifier */
	Identifier();
	Identifier(int32_t l ) {_id =l;}
	Identifier(uint32_t l ) {_id =l;} 
	Identifier(uint64_t l ) {_id =l;}
	Identifier(double l ) {_id = (uint64_t)l;}
	Identifier(uint8_t user_id );
	Identifier(const Identifier & ids);
	/** @} */ 	
public:
	/** @name 静态生成函数集  
	*  @{
	*/
	/**  生成一个值为0的ID对象
	*	 @param 无
	*   @return Identifier对象     
	*/
	static Identifier zero() {	return Identifier(0);}
	/**  生成一个唯一的原始ID值，为一个64位整数
	*	 @param 无
	*   @return 返回原始64位ID    
	*/
	static raw_type  generate() {
		Identifier<BITS> i;
		return i.get();
	}
	/** @} */  
public:
	/** @name 获取Identifier的值  
	*  @{
	*/
	inline uint64_t  get() { return _id; }
	inline const uint64_t  get() const { return _id; }
	inline uint32_t getTimestamp(){ return _sid.time_stamp;}
	inline uint8_t getUserID() { return _sid.user_id;}
	inline uint8_t * getBytes(){return _bid;}
	/** @} */ 
public:
	/** @name 赋值等号重载  
	*  @{
	*/
	Identifier operator = (const Identifier&  ids);
	Identifier operator = (uint64_t  ids);
	Identifier operator = (int32_t  ids); 
	Identifier operator = (uint32_t   ids);
	/** @} */ 

	/** @name 类型转换操作重载  
	*  @{
	*/
	operator uint64_t  () { return _id; }
	operator uint32_t  () { return (uint32_t)_id; } 
	operator int32_t() { return (int32_t)_id; }
	uint64_t * operator &() { return &_id; }
	/** @} */ 	
 
};
 
typedef Identifier<64,char> Identifier64;

end_gtl_namespace

#include "identifier.inl"
