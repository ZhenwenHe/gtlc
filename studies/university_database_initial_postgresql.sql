
drop table SC;
drop table TC;
drop table Course;
drop table Teacher;
drop table Student;
drop table Department; 

/*系的信息表 Department（Dno,Dname,Daddress）*/
create table Department(
	Dno int,
	Dname varchar(50),
	Daddress varchar(50),
	primary key (Dno)
	);
insert into Department(Dno,Dname,Daddress) values(1,'地球科学学院','主楼东');
insert into Department(Dno,Dname,Daddress) values(2,'资源学院','主楼西');
insert into Department(Dno,Dname,Daddress) values(3,'材化学院','材化楼');
insert into Department(Dno,Dname,Daddress) values(4,'环境学院','文华楼');
insert into Department(Dno,Dname,Daddress) values(5,'工程学院','水工楼');
insert into Department(Dno,Dname,Daddress) values(6,'地球物理与空间信息学院','物探楼');
insert into Department(Dno,Dname,Daddress) values(7,'机械与电子信息学院','教二楼');
insert into Department(Dno,Dname,Daddress) values(8,'经济管理学院','经管楼');
insert into Department(Dno,Dname,Daddress) values(9,'外语学院','北一楼');
insert into Department(Dno,Dname) values(10,'信息工程学院');
insert into Department(Dno,Dname,Daddress) values(11,'数学与物理学院','基委楼');
insert into Department(Dno,Dname,Daddress) values(12,'珠宝学院','珠宝楼');
insert into Department(Dno,Dname,Daddress) values(13,'政法学院','政法楼');
insert into Department(Dno,Dname,Daddress) values(14,'计算机学院','北一楼');
insert into Department(Dno,Dname) values(15,'远程与继续教育学院');
insert into Department(Dno,Dname) values(16,'国际教育学院');
insert into Department(Dno,Dname,Daddress) values(17,'体育部','体育馆');
insert into Department(Dno,Dname,Daddress) values(18,'艺术与传媒学院','艺传楼');
insert into Department(Dno,Dname,Daddress) values(19,'马克思主义学院','保卫楼');
insert into Department(Dno,Dname,Daddress) values(20,'江城学院','江城校区');

/*学生信息表Student（Sno, Sname, Ssex, Sage, Dno）*/
create table Student(
	Sno char(11) ,
	Sname varchar(50),
	Ssex char(2),
	Sage int,
	Dno int,
	primary key (Sno),
	foreign key (Dno) references Department(Dno)
	);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091000231','吕岩','M',18,14);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091004391','颜荣','M',19,14);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091001598','王海涛','M',20,14);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091003085','袁恒','M',18,14);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091000863','孟飞','M',17,14);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091000934','罗振俊','M',19,8);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091000961','曾雪君','F',18,8);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091000983','巴翔','M',19,8);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091001175','周雷','M',18,8);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091001261','马欢','M',17,8);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091001384','陈亮','M',20,8);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20081003492','易家新','M',19,5);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20081001197','李子聪','M',17,5);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20081001266','蔡景学','F',19,5);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20081001888','赵林云','F',20,5);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20091000481','姜北','M',17,5);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20101000199','孙中孝','M',18,11); 
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20101000424','杨光','M',17,11);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20101000481','张永强','M',16,11);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20101000619','陈博','M',20,11);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20101000705','汤文盼','M',18,11);
insert into Student(Sno, Sname, Ssex, Sage, Dno) values('20101000802','苏海恩','M',17,11);


/*教师信息表Teacher （Tno, Tname, Ttitle, Dno）*/
create table Teacher(
	Tno int primary key,
	Tname varchar(50) ,
	Ttitle varchar(50),
	Dno int ,
	foreign key (Dno) references Department(Dno)
	);
insert into Teacher(Tno, Tname, Ttitle, Dno) values(1,'何小峰','副教授',14);
insert into Teacher(Tno, Tname, Ttitle, Dno) values(2,'刘刚才','教授',14);
insert into Teacher(Tno, Tname, Ttitle, Dno) values(3,'李星星','教授',11);
insert into Teacher(Tno, Tname, Ttitle, Dno) values(4,'翁平正','讲师',14);
insert into Teacher(Tno, Tname, Ttitle, Dno) values(5,'李川川','讲师',14);
insert into Teacher(Tno, Tname, Ttitle, Dno) values(6,'王媛媛','讲师',14);
insert into Teacher(Tno, Tname, Ttitle, Dno) values(7,'孔夏芳','副教授',14);

/*课程信息表 Course  （Cno, Cname, Cpno, Ccredit)*/
create table Course(
	Cno int primary key ,
	Cname varchar(50),
	Cpno int ,
	CCredit int,
	foreign key(Cpno) references Course(Cno)
	);
insert into Course(Cno, Cname, Ccredit) values(2,'高等数学',8);
insert into Course(Cno, Cname, Ccredit) values(6,'C语言程序设计',4);
insert into Course(Cno, Cname, Ccredit) values(7,'大学物理',8);
insert into Course(Cno, Cname, Ccredit) values(8,'大学化学',3);
insert into Course(Cno, Cname, Ccredit) values(10,'软件工程',2);
insert into Course(Cno, Cname, Ccredit) values(12,'美国简史',2);
insert into Course(Cno, Cname, Ccredit) values(13,'中国通史',6);
insert into Course(Cno, Cname, Ccredit) values(14,'大学语文',3);
insert into Course(Cno, Cname, Cpno, Ccredit) values(5,'数据结构',6,4);
insert into Course(Cno, Cname, Cpno, Ccredit) values(4,'操作系统',5,4);
insert into Course(Cno, Cname, Cpno, Ccredit) values(1,'数据库原理',5,4);
insert into Course(Cno, Cname, Cpno, Ccredit) values(3,'信息系统',1,2);
insert into Course(Cno, Cname, Cpno, Ccredit) values(9,'汇编语言',6,2);
insert into Course(Cno, Cname, Cpno, Ccredit) values(11,'空间数据库',1,3);


/*学生选课表SC（Sno,Cno,Grade）*/
create table SC(
	Sno char(11) ,
	Cno int, 
	Grade int,
	primary key(Sno, Cno),
	foreign key(Sno) references Student(Sno),
	foreign key (Cno) references Course(Cno)
	);
insert into SC values('20091003085',1,90);
insert into SC values('20091000863',1,98);
insert into SC values('20091000934',1,89);
insert into SC values('20091000961',1,85);
insert into SC values('20081001197',1,79);
insert into SC values('20081001266',1,97);
insert into SC values('20081001888',1,60);
insert into SC values('20091000481',1,78);
insert into SC values('20101000199',1,65);
insert into SC values('20101000424',1,78);
insert into SC values('20101000481',1,69);
insert into SC values('20091000863',6,90);
insert into SC values('20091000934',6,90);
insert into SC values('20091000961',6,87); 
 
/*教师授课表TC（Tno,Cno,Site）*/
create table TC(
	Tno int ,
	Cno int,
	Site varchar(50),
	primary key (Tno,Cno),
	foreign key(Tno) references Teacher(Tno),
	foreign key (Cno) references Course(Cno)
	);
insert into TC values(1,1,'教一楼407');
insert into TC values(1,6,'教一楼307');
insert into TC values(2,10,'教二楼217');
insert into TC values(3,2,'教三楼507');
insert into TC values(4,5,'教三楼208');
insert into TC values(6,3,'综合楼207');
insert into TC values(7,4,'教二楼817');
insert into TC values(5,9,'教一楼207');