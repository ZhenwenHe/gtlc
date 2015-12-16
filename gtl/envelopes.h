#pragma  once
 #include "envelope.h"
begin_gtl_namespace
template<
	typename ElemType,
	int NUMDIMS,
	typename ELEMENT=char,
	typename TRAITS=std::char_traits<ELEMENT>,
	typename ALLOC=std::allocator<ELEMENT> >
class Envelopes:public Object<ELEMENT,TRAITS,ALLOC>,public std::vector<Envelope<ElemType,NUMDIMS,ELEMENT,TRAITS,ALLOC> > {
public:
	typedef Envelope<ElemType,NUMDIMS,ELEMENT,TRAITS,ALLOC>  envelope_type;
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "Envelopes";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
public:
	void write (const std::basic_string<ELEMENT,TRAITS,ALLOC> & filename ){
		assert(NUMDIMS==4);
		//1. Dimension 
		//2. Objects Number
		//3. Order
		//2. T X Y Z[LOW]
		//3. T X Y Z[HIGH]
		std::locale _loc = std::locale::global(std::locale(""));

		std::ofstream ofs;
		ofs.open(filename.c_str(),std::ios_base::out|std::ios_base::trunc); 
		size_t k,i;
		if (ofs)
		{				
			ofs<< NUMDIMS << std::endl;
			ofs<< size() << std::endl;
			
			for( i=0;i<size();i++)
			{
				reference it = at(i); 
				ofs<<i<<std::endl; 
				 
				for( k=0;k<NUMDIMS-1;k++){
					ofs<<it.getLowerBound(k)<<" ";
				}
				ofs<<it.getLowerBound(k)<<std::endl;

				for( k=0;k<NUMDIMS-1;k++){
					ofs<<it.getUpperBound(k)<<" ";
				}
				ofs<<it.getUpperBound(k)<<std::endl;
			}
		}

		ofs.close();
		std::locale::global(_loc); 

	}

	void read(const std::basic_string<ELEMENT,TRAITS,ALLOC> & filename){
		//1. Dimension 
		//2. Objects Number
		//3. Order
		//2. T X Y Z[LOW]
		//3. T X Y Z[HIGH]
		assert(NUMDIMS==4 || NUMDIMS==3);
		std::locale _loc = std::locale::global(std::locale(""));//设置环境为系统环境  

		std::ifstream ifs;

		ifs.open(filename.c_str(),std::ios_base::in|std::ios_base::_Nocreate);
		char sz[MAX_PATH];	

		if (ifs)
		{				
			ifs.getline(sz,MAX_PATH);
			int dims=stringToNumber<int,char>(sz);
			assert(dims==NUMDIMS);
			ifs.getline(sz,MAX_PATH);
			int objectsNumber=stringToNumber<int,char>(sz);
			resize(objectsNumber);
 
			int i,k;
			for(i=0;i<objectsNumber;i++)
			{
				ifs.getline(sz,MAX_PATH);//order

				reference it = at(i);
				if(NUMDIMS==4){
					for(k=0;k<NUMDIMS-1;k++){
						ifs.getline(sz,MAX_PATH,' ');
						it.setLowerBound(k,stringToNumber<double,char>(sz));
					}
					ifs.getline(sz,MAX_PATH);
					it.setLowerBound(k,stringToNumber<double,char>(sz));


					for(k=0;k<NUMDIMS-1;k++){
						ifs.getline(sz,MAX_PATH,' ');
						it.setUpperBound(k,stringToNumber<double,char>(sz));
					}
					ifs.getline(sz,MAX_PATH);
					it.setUpperBound(k,stringToNumber<double,char>(sz));
				}
				else{

					for(k=0;k<NUMDIMS;k++){
						ifs.getline(sz,MAX_PATH,' ');
						it.setLowerBound(k,stringToNumber<double,char>(sz));
					}

					ifs.getline(sz,MAX_PATH);//skip z
					//it.setLowerBound(k,stringToNumber<double,char>(sz));


					for(k=0;k<NUMDIMS;k++){
						ifs.getline(sz,MAX_PATH,' ');
						it.setUpperBound(k,stringToNumber<double,char>(sz));
					}

					ifs.getline(sz,MAX_PATH);//skip z
					//it.setUpperBound(k,stringToNumber<double,char>(sz));
				}
				
			}		

		}
		ifs.close();
		std::locale::global(_loc);//还原环境  
	}

	inline void transform(const envelope_type & e1, const envelope_type & e2){
		for(iterator it = begin();it!=end();it++){
			it->transform(e1,e2);
		}
	}
};
 
end_gtl_namespace 