#include "stdafx.h" 
#include <vector>
#include <string>
#include "timer.h"
#include "random.h"  
#include "spatiotemporalgenerator.h"
extern gtl::Console<char> g_console;
extern std::string _database;
extern gtl::Envelope<double,4> _cubeenvelope;
extern int _cube_size_length ;
void generate_random_points(const int maxpoints, const double minv[], const double maxv[] ,gtl::MovingPoints<double,4> & pnts){
	gtl::RandomNumbers<double> t(maxpoints,minv[0],maxv[0]);
	gtl::RandomNumbers<double> x(maxpoints,minv[1],maxv[1]);
	gtl::RandomNumbers<double>  y(maxpoints,minv[2],maxv[2]);
	gtl::RandomNumbers<double>  z(maxpoints,minv[3],maxv[3]);

	for(int i=0;i<maxpoints;i++)
	{
		gtl::MovingPoint<double,4> *  p = new gtl::MovingPoint<double,4>();

		p->start(t.at(i));
		p->end(p->start());
		p->setPosition(0,x.at(i));
		p->setPosition(1,y.at(i));
		p->setPosition(2,z.at(i));
		p->setIdentifier(i);
		pnts.add(p);
	}
}

void generate_query_envelopes(){
	gtl::Envelopes<double,4> qes;
	double curpos[4];
	int i,j;
	int runtimes=100;
	qes.resize(runtimes);
	for(int k =1;k<=100;k++){
		//generate the query envelopes
		for(i=0;i<runtimes;i++){
			gtl::Envelopes<double,4>::reference it = qes.at(i);
			for( j=0;j<4;j++)
				curpos[j]=rand()% _cube_size_length;
			it=gtl::Envelope<double,4>(curpos,10*k);
		}
		qes.write(_database+"envelopes"+gtl::numberToString<char,int>(10*k)+".ef");
	}
}



void prepare_test_data() {
	generate_query_envelopes();
	//gtl::Envelopes<double,4> qes;
	//qes.read(_database+"envelopes10.ef");
	//1. import delft data
	gtl::Trajectories<double,4> trjs;
	//trjs.import(_database+"zp_all_pedestrians_wgs84.txt");
	gtl::SpatiotemporalReader<double,4>::importTrajectories(_database+"zp_all_pedestrians_wgs84.txt",trjs);
	trjs.normalize(0,10000);
	//2. store it in moving object file  
	//trjs.write(_database+"delft.mtf");
	gtl::SpatiotemporalWriter<double,4>::writeTrajectories(_database+"delft.mtf",trjs);
	//3. change them into segments 
	gtl::MovingSegments<double,4> segs;
	trjs.split(segs);
	//4. store them in moving segment file
	//segs.write(_database+"delft.msf");
	gtl::SpatiotemporalWriter<double,4>::writeSegments(_database+"delft.msf",segs);
	segs.clear();
	//5. change them into moving points
	gtl::MovingPoints<double,4> pnts;
	trjs.split(pnts);
	//6. store them in moving point file
	//pnts.write(_database+"delft.mpf");
	gtl::SpatiotemporalWriter<double,4>::writePoints(_database+"delft.mpf",pnts);
	pnts.clear();
	//7. clear the trajectories
	trjs.clear();

	//8. generate random moving points
	double minData[4] = {0,0,0,0};
	double maxData[4] = {3600*5,10240,10240,10240};
	double low_velocity[3]={0,0,0};
	double high_velocity[3]={3,3,3};
	gtl::SpatiotemporalGenerator<double,4> stg(
		minData[0],
		maxData[0],// 1 hour
		minData+1,
		maxData+1,
		low_velocity,
		high_velocity,// maximum speed is 3m/s
		60 // 60 seconds
		);
	stg.generateTrajectories(150,200,trjs);
	trjs.normalize(0,10000);
	//9. store it in moving object file  
	//trjs.write(_database+"random.mtf");
	gtl::SpatiotemporalWriter<double,4>::writeTrajectories(_database+"random.mtf",trjs);
	//10. change them into segments 
	trjs.split(segs);
	//11. store them in moving segment file
	//segs.write(_database+"random.msf");
	gtl::SpatiotemporalWriter<double,4>::writeSegments(_database+"random.msf",segs);
	segs.clear();
	//12. change them into moving points
	trjs.split(pnts);
	//13. store them in moving point file
	//pnts.write(_database+"random.mpf");
	gtl::SpatiotemporalWriter<double,4>::writePoints(_database+"random.mpf",pnts);
	pnts.clear();
	//15. clear the trajectories
	trjs.clear();
}

unsigned long long  calculateNodes(int n)
{
	unsigned long long s=1;
	unsigned long long s1=1;
	for(int i=1;i<n;i++)
	{
		s1=1;
		for(int j=0;j<i;j++)
		{
			s1=s1*16;
		}
		s+= s1;
	}
	return s;	
}
