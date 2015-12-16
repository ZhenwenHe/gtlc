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

#pragma  once
#include "object.h"


begin_gtl_namespace

template<
	typename T,
	typename  ELEMENT=char,
	typename TRAITS=std::char_traits<ELEMENT>,
	typename ALLOC=std::allocator<ELEMENT> 
>
class Matrix: public Object<ELEMENT,TRAITS,ALLOC>,
	public std::vector<T> //row-major matrix			 
{
public:
	typedef  std::vector<T> MatrixRowT;
	typedef  std::vector<T> MatrixColT;
	typedef  size_t         RowIndexT;
	typedef  size_t         ColIndexT;
	typedef  struct{
		RowIndexT r;
		ColIndexT c;
	}                      IndexCoord;
public: 
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "Matrix";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}

private:
	size_t	cols;
public:
	Matrix(){
		cols=0;
	}
	Matrix (size_t r,size_t c):std::vector<T>(r*c){
		cols=c;		
	}
	Matrix (size_t r,size_t c,const T & t):std::vector<T>(r*c,t){
		cols=c;	
	}
	Matrix (std::vector<T> & data,size_t row, size_t col):std::vector<T>(row*col){
		assert(data.size()==row*col);
		cols=col;	
		std::copy(data.begin(),data.end(),((std::vector<T>*)(this))->begin());
	}
	Matrix (const Matrix<T> & m){
		cols=m.cols;	
		std::vector<T>::resize(m.size());
		std::copy(m.begin(),m.end(),begin());
	}
	void resize(size_t r,size_t c,T t){
		cols=c;	
		std::vector<T>::resize(r*c,t);
	}
	void resize(size_t r,size_t c){
		cols=c;	
		std::vector<T>::resize(r*c);
	}
	
	void clear(){
		cols=0;	
		std::vector<T>::clear();
	}
	inline size_t sizeRows() const {
		return std::vector<T>::size()/cols;
	}
	inline size_t sizeCols() const {
		return  cols;
	}
	void eraseRow(RowIndexT row){
		std::vector<T>::erase(begin()+row*cols,begin()+(row+1)*cols);
	}
	void set(RowIndexT r,ColIndexT c,const T & t){
		*((begin() +(r*cols+c)) = t;
	}
	inline T &  get(RowIndexT r,ColIndexT c){
		//return ((std::vector<T>&)(*this))[r*cols+c] ;
		return *(begin()+r*cols+c);
	}
	inline T & operator () (RowIndexT r,ColIndexT c){
		//return ((std::vector<T>&)(*this))[r*cols+c] ;
		return *(begin()+r*cols+c);
	}
	inline T & operator () (size_t  i){
		//return ((std::vector<T>&)(*this))[r*cols+c] ;
		return *(begin()+i);
	}
public:
	void setZero(){ 
		for(iterator it=begin();it!=end();it++){
			*it= 0;
		}
	}
	void setIdentity(){
		size_t rows = sizeRows();
		for(RowIndexT r=0;r<rows;r++)
		{
			for( ColIndexT c=0;c<cols;c++){
				if(c==r)
					*(begin()+r * cols +c)=1;
				else
					*(begin()+r * cols +c)=0;
			}
		}
	}
	/*!
	\brief sets the diagonal of the matrix to \a a

	Sets the diagonal points of the matrix to \a a. The diagonal 
	points are \a (0,0),(1,1),(2,2),etc.

	\param a  the value to set the diagonal to

	\author Philippe Lavoie 
	\date 24 January 1997
	*/
	void setDiagonal(const T a)
	{
		int i, iend;

		iend = sizeRows();
		if ( iend > sizeCols() )
			iend = sizeCols();

		for (i = iend-1; i >=0; --i)
			set(i,i,a);
	}

	Matrix<T>&  submatrix(RowIndexT r0,ColIndexT c0, RowIndexT r1,ColIndexT c1,Matrix<T> & resultMatrix){
		resultMatrix.resize(abs(r1-r0),abs(c1-c0));
		size_t rows = sizeRows();
		for(size_t r=r0;r<r1+1;r++)
		{
			for( size_t c=c0;c<c1;c++){
				resultMatrix.set(r-r0,c-c0, *(begin()+r * cols +c));
			}
		}
		return resultMatrix;
	}
	Matrix<T>&  submatrix(RowIndexT r,ColIndexT c,Matrix<T> & resultMatrix){
		return submatrix(0,0,r,c,resultMatrix);
	}
	void copy(const Matrix<T> & m){
		size_t s = m.size();
		if(size()!=s){
			std::vector<T>::resize(s);
		}
		cols = m.cols;
		std::copy(m.begin(),m.end(),begin());
	}
public:
	

	void getRow(RowIndexT i, MatrixRowT & mr ){
		mr.resize(cols);
		std::copy(begin()+i*cols,begin()+(i+1)*cols,mr.begin());
	}
	void getCol(ColIndexT c, MatrixColT & mc ){
		size_t rows = sizeRows();
		mc.resize(rows);
		for(ColIndexT i =0 ;i<rows;i++){
			mc[i]=*(begin()+i*cols+c);
		}
	}
	void setRow(RowIndexT i, MatrixRowT & mr ){
		std::copy(mr.begin(),mr.end(),begin()+i*cols);
	}
	void setCol(ColIndexT c, MatrixColT & mc ){
		size_t rows = sizeRows();
		for(ColIndexT i =0 ;i<rows;i++){
			*(begin()+i*cols+c)=mc[i];
		}
	}
	void swapRows(RowIndexT r1,RowIndexT r2){
		T t;
		for(size_t i=0;i<cols;i++){
			t=*(begin()+r1*cols+i);
			*(begin()+r1*cols+i)=*(begin()+r2*cols+i);
			*(begin()+r2*cols+i)=t;
		}
	}
	void swapCols(ColIndexT c1,ColIndexT c2){
		T t;
		size_t rows = sizeRows();
		for(size_t i=0;i<rows;i++){
			t=*(begin()+i*cols+c1);
			*(begin()+i*cols+c1)=*(begin()+i*cols+c2);
			*(begin()+i*cols+c2)=t;
		}
	}
	void tranpose(Matrix<T> & dest_out){
		size_t rows = sizeRows();
		dest_out.resize(cols,rows);
		for(RowIndexT r=0;r<rows;r++)
		{
			for( ColIndexT c=0;c<cols;c++){
				dest_out.set(c,r,get(r,c));
			}
		}
	}
	void tranpose(){
		size_t rows = sizeRows();
		Matrix<T> dest_out(cols,rows);
		for(RowIndexT r=0;r<rows;r++)
		{
			for( ColIndexT c=0;c<cols;c++){
				dest_out(c,r)=(*this)(r,c);
			}
		}
		copy(dest_out);
		dest_out.clear();
	}
public:
	Matrix<T>& operator = (const Matrix<T> & m){
		cols=m.cols;	
		std::vector<T>::resize(m.size());
		std::copy(m.begin(),m.end(),begin());
		return *this;
	}

	bool inverse( )//Äæ¾ØÕó±ä»»
	{
		if(cols!=sizeRows())
		{
			return false;
		}
		else
		{
			int *is,*js,i,j,k,l,u,v;
			T d,p;
			std::vector<T> &a   =(std::vector<T> &)(*this);
			int n = cols;
			is=new int[n];
			js=new int[n];
			for (k=0; k<=n-1; k++)
			{ 
				d=0.0;
				for (i=k; i<=n-1; i++)
					for (j=k; j<=n-1; j++)
					{ 
						l=i*n+j; p=fabs(a[l]);
						if (p>d) { d=p; is[k]=i; js[k]=j;}
					}
					if (d+1.0==1.0)
					{ 
						free(is); free(js);
						return false;
					}
					if (is[k]!=k)
						for (j=0; j<=n-1; j++)
						{ 
							u=k*n+j; v=is[k]*n+j;
							p=a[u]; a[u]=a[v]; a[v]=p;
						}
						if (js[k]!=k)
							for (i=0; i<=n-1; i++)
							{ 
								u=i*n+k; v=i*n+js[k];
								p=a[u]; a[u]=a[v]; a[v]=p;
							}
							l=k*n+k;
							a[l]=1.0/a[l];
							for (j=0; j<=n-1; j++)
								if (j!=k)
								{ u=k*n+j; a[u]=a[u]*a[l];}
								for (i=0; i<=n-1; i++)
									if (i!=k)
										for (j=0; j<=n-1; j++)
											if (j!=k)
											{ 
												u=i*n+j;
												a[u]=a[u]-a[i*n+k]*a[k*n+j];
											}
											for (i=0; i<=n-1; i++)
												if (i!=k)
												{ 
													u=i*n+k; a[u]=-a[u]*a[l];
												}
			}
			for (k=n-1; k>=0; k--)
			{ 
				if (js[k]!=k)
					for (j=0; j<=n-1; j++)
					{ 
						u=k*n+j; v=js[k]*n+j;
						p=a[u]; a[u]=a[v]; a[v]=p;
					}
					if (is[k]!=k)
						for (i=0; i<=n-1; i++)
						{ 
							u=i*n+k; v=i*n+is[k];
							p=a[u]; a[u]=a[v]; a[v]=p;
						}
			}
			delete [] is;
			delete [] js;
			return true;
		}
	}
public:
	T & minElement(){
		T s = * begin();
		for(size_t i=0,iterator it = begin();it!=end();it++,i++){
			if( s> *it) s= *it;
		}
		return s;
	}
	T & maxElement(){
		T s = * begin();
		for(size_t i=0,iterator it = begin();it!=end();it++,i++){
			if( s< *it) s= *it;
		}
		return s;
	}
	void maxmin(T & out_max,T & out_min){
		out_max = * begin();
		out_min = * begin();
		for(size_t i=0,iterator it = begin();it!=end();it++,i++){
			if( out_max< *it) out_max= *it;
			if( out_min> *it) out_min= *it;
		}
	}

};

template<class T>
static Matrix<T> operator+(const Matrix<T> & m1,const Matrix<T> & m2){
	Matrix<T> m(m1.sizeRows(),m1.sizeCols());
	for(Matrix<T>::iterator it = m.begin(),
		it1=m1.begin(),
		it2=m2.begin();it!=m.end();it++,it1++,it2++){
		*it = *it1+*it2;
	}
	return m;
}

template<class T>
static Matrix<T> operator+(const Matrix<T> & m1,T & m2){
	Matrix<T> m(m1.sizeRows(),m1.sizeCols());
	for(Matrix<T>::iterator it = m.begin(),
		it1=m1.begin();it!=m.end();it++,it1++){
			*it = *it1+m2;
	}
	return m;
}

template<class T>
static Matrix<T> operator-(const Matrix<T> & m1,const Matrix<T> & m2){
	Matrix<T> m(m1.sizeRows(),m1.sizeCols());
	for(Matrix<T>::iterator it = m.begin(),
		it1=m1.begin(),
		it2=m2.begin();it!=m.end();it++,it1++,it2++){
			*it = *it1-*it2;
	}
	return m;
}

template<class T>
static Matrix<T> operator-(const Matrix<T> & m1,T & m2){
	Matrix<T> m(m1.sizeRows(),m1.sizeCols());
	for(Matrix<T>::iterator it = m.begin(),
		it1=m1.begin();it!=m.end();it++,it1++){
			*it = *it1-m2;
	}
	return m;
}


template<class T>
static Matrix<T> operator*(const Matrix<T> & m11,const Matrix<T> & m22){
	Matrix<T> & m1 =  (Matrix<T> &) m11;
	Matrix<T> & m2 =  (Matrix<T> &) m22;
	size_t rows = m1.sizeRows();
	size_t cols = m2.sizeCols();
	size_t k = m1.sizeCols();
	Matrix<T> m(rows,cols);
	
	for(int r = 0;r<rows;r++){
		for(int c=0;c<cols;c++){
			T s = 0;
			for(size_t i=0;i<k;i++){
				s+=m1.get(r,i)*m2.get(i,c);
			}
			m.set(r,c,s);
		}
	}
	return m;
}

template<class T>
static Matrix<T> operator*(const Matrix<T> & m1,T & m2){
	Matrix<T> m(m1.sizeRows(),m1.sizeCols());
	for(Matrix<T>::iterator it = m.begin(),
		it1=m1.begin();it!=m.end();it++,it1++){
			*it = *it1*m2;
	}
	return m;
}

template<class T>
static Matrix<T> operator/(const Matrix<T> & m1,const Matrix<T> & m2){
	Matrix<T> m(m2);
	m.inverse();
	return m1*m;
}

template<class T>
static Matrix<T> operator/(const Matrix<T> & m1,T & m2){
	Matrix<T> m(m1.sizeRows(),m1.sizeCols());
	for(Matrix<T>::iterator it = m.begin(),
		it1=m1.begin();it!=m.end();it++,it1++){
			*it = *it1/m2;
	}
	return m;
}


/*!
\class LUMatrix matrixMat.h matrix/matrixMat.h
\brief a class for the LU decomposition of a matrix

\latexonly
This class was adapted from a class in the Kalman library
written by Skip Carter (\verb|skip@taygeta.oc.nps.navy.mil|)
available from 
\verb|ftp:://usc.edu/pub/C-numanal/kalman.tar.gz|
\endlatexonly
\htmlonly
This class was adapted from a class in the 
<A HREF="ftp:://usc.edu/pub/C-numanal/kalman.tar.gz">Kalman library</A>
written by Skip Carter 
(<A HREF="mailto:skip@taygeta.oc.nps.navy.mil">skip@taygeta.oc.nps.navy.mil</A>
\endhtmlonly

It performs the LU decomposition of a matrix. It can be
used to find it's inverse and to solve linear systems.

This class can only be used with floating point values
(float or double).

\author Skip Carter
\author Philippe Lavoie 
\date 22 October 1997
*/
template <class T>
class LUMatrix : public Matrix<T> 
{
public:
	int cols(){ return sizeCols();}
	int rows(){return sizeRows();}
	T & elem(int i,int j){return get(i,j);}

	LUMatrix(int r, int c) : Matrix<T>(r,c),pivot(pivot_) { pivot_.resize(r) ; }

	LUMatrix()  : Matrix<T>(),pivot(pivot_)    {; }

	LUMatrix(const LUMatrix<T>& lu): Matrix<T>(lu),pivot(pivot_) { pivot_ = lu.pivot_;}

	LUMatrix(const Matrix<T>& a): Matrix<T>(a.sizeRows(),a.sizeCols()),pivot(pivot_) { decompose(a) ; }

	void resize(const int r, const int c) { Matrix<T>::resize(r,c) ; pivot_.resize(r) ; }

	LUMatrix& operator=(const LUMatrix<T>&a){
		resize(a.rows(),a.cols()) ;
		for(int i=0;i<rows();++i)
			for(int j=0;j<cols();++j)
				elem(i,j) = a(i,j) ;
		pivot_ = a.pivot_ ;
		return *this ;
	}

	LUMatrix& decompose(const Matrix<T> &a_matrix){
		Matrix<T> & a = (Matrix<T> & )a_matrix;
		int i, j, k, l, kp1, nm1, n;
		T t, q;
		double den, ten;

		n = a.sizeRows();

		resize(n,n) ;

		if(a.sizeRows()!=a.sizeCols()){
			return * this;
		}

		//	lu = a;	 must do it by copying or LUFACT will be recursively called !
		for(i=0;i<n;++i)
			for(j=0;j<n;++j)
				elem(i,j) = a(i,j) ;

		errval = 0;
		nm1 = n - 1;
		for (k = 0; k < n; k++)
			pivot_[k] = k ;

		sign = 1 ;

		if (nm1 >= 1)	/* non-trivial problem */
		{
			for (k = 0; k < nm1; k++)
			{
				kp1 = k + 1;
				// partial pivoting ROW exchanges
				// -- search over column 

				ten = fabs(a(k,k));

				l = k;
				for (i = kp1; i < n; i++)
				{
					den = fabs(a(i,k));
					if ( den > ten )
					{
						ten = den;
						l = i;
					}
				}
				pivot_[k] = l;

				if ( elem(l,k) != 0.0 )
				{			// nonsingular pivot found 
					if (l != k ){	// interchange needed 
						for (i = k; i < n; i++)
						{
							t = elem(l,i) ;
							elem(l,i) = elem(k,i) ;
							elem(k,i) = t ; 
						}
						sign = -sign ;
					}
					q =  elem(k,k);	/* scale row */
					for (i = kp1; i < n; i++)
					{
						t = - elem(i,k)/q;
						elem(i,k) = t;
						for (j = kp1; j < n; j++)
							elem(i,j) += t * elem(k,j);
					}
				}
				else		/* pivot singular */
					errval = k;
			}

		}

		pivot_[nm1] = nm1;
		if (elem(nm1,nm1) == 0.0)
			errval = nm1;  
		return *this;
	}

	T determinant() {
		T det = elem(0,0) ;
		for(int i=1;i<rows();++i)
			det *= elem(i,i) ;
		return det * (T)sign ;
	}

	Matrix<T> inverse() {
		if ( rows() != cols() )
		{
			return *this;
		}

		Matrix<T> inverse ;
		inverseIn(inverse) ;

		return inverse;
	}

	void inverseIn(Matrix<T>& inv){
		T ten;
		int i, j, k, l, kb, kp1, nm1, n, coln;

		if ( rows() != cols() )
		{
			return ;
		}

		n = coln = rows();


		inv = *this ;

		nm1 = n - 1;

		// inverse U 
		for (k = 0; k < n; k++)
		{
			inv(k,k) = ten = 1.0 / inv(k,k) ;
			ten = -ten;
			for (i = 0; i < k; i++)
				inv(i,k) *= ten;
			kp1 = k + 1;
			if (nm1 >= kp1)
			{
				for (j = kp1; j < n; j++)
				{
					ten = inv(k,j) ;
					inv(k,j) = 0.0 ;
					for (i = 0; i < kp1; i++)
						inv(i,j) += ten * inv(i,k) ;	      
				}
			}
		}

		// INV(U) * INV(L)   
		if (nm1 >= 1)
		{
			std::vector<T> work(n) ;
			//error.memory( work.memory() );

			for (kb = 0; kb < nm1; kb++)
			{
				k = nm1 - kb - 1;
				kp1 = k + 1;
				for (i = kp1; i < n; i++)
				{
					work[i] = inv(i,k) ;
					inv(i,k) = 0.0;
				}
				for (j = kp1; j < n; j++)
				{
					ten = work[j];
					for (i = 0; i < n; i++)
						inv(i,k) += ten * inv(i,j) ;
				}
				l = pivot[k];
				if (l != k)
					for (i = 0; i < n; i++)
					{
						ten = inv(i,k) ;
						inv(i,k) = inv(i,l);
						inv(i,l) = ten;
					}
			}
		} 
	}

	const std::vector<int>& pivot ;

private:
	std::vector<int> pivot_ ;

protected:
	int errval ;
	int sign ;
};


/*!
\class SVDMatrix  matrixMat.h matrix/matrixMat.h
\brief A matrix for the SVD decomposition

\latexonly
This matrix was adapted from the Numerical Math Class Library
developed by Oleg Kiselyov and available from
\verb|http://www.lh.com/oleg/ftp/packages.html|.
\endlatexonly
\htmlonly
This matrix was adapted from the 
<A HREF="http://www.lh.com/oleg/ftp/packages.html">Numerical Math 
Class Library</A>
developed by Oleg Kiselyov.
\endhtmlonly

This class can only be used with floating point values
(float or double).

\latexonly
The following comments are from Oleg Kiselyov.

Singular Value Decomposition of a rectangular matrix
$ A = U  Sig  V'$
where matrices $U$ and $V$ are orthogonal and $Sig$ is a 
diagonal matrix.

The singular value decomposition is performed by constructing 
an SVD  object from an $M\times N$ matrix $A$ with $M \ge N$
(that is, at least as many rows as columns). Note, in case 
$M > N,$ matrix $Sig$ has to be a $M \times N$ diagonal
matrix. However, it has only $N$ diagonal elements, which 
we store in a vector sig.

{\bf Algorithm:}	Bidiagonalization with Householder 
reflections followed by a modification of a QR-algorithm. 
For more details, see G.E. Forsythe, M.A. Malcolm, C.B. Moler 
Computer methods for mathematical computations, Prentice-Hall, 
1977.  However, in the present implementation, matrices $U$ 
and $V$ are computed right away rather than delayed until 
after all Householder reflections.

This code is based for the most part on a Algol68 code I 
(Oleg Kiselyov) wrote ca. 1987.

Look at the source code for more information about the 
algorithm.
\endlatexonly

\htmlonly
<P>
The following comments are from Oleg Kiselyov.
</P>
<P>
Singular Value Decomposition of a rectangular matrix
<code>A=U Sig V'</code>
where matrices U and V are orthogonal and Sig is a 
diagonal matrix.
</p>
<P>
The singular value decomposition is performed by constructing 
an SVD  object from an M*N matrix A with M \ge N
(that is, at least as many rows as columns). Note, in case 
M &gt; N, matrix  Sig has to be a M*N diagonal
matrix. However, it has only N diagonal elements, which 
we store in a vector sig.
</p>
<P>
<strong>Algorithm:</strong> Bidiagonalization with Householder 
reflections followed by a modification of a QR-algorithm. 
For more details, see G.E. Forsythe, M.A. Malcolm, C.B. Moler 
Computer methods for mathematical computations, Prentice-Hall, 
1977.  However, in the present implementation, matrices  U
and V are computed right away rather than delayed until 
after all Householder reflections.
</p>
<p>
This code is based for the most part on a Algol68 code I 
(Oleg Kiselyov) wrote ca. 1987.
</P>
<p>
Look at the source code for more information about the 
algorithm.
</p>
\endhtmlonly

\author Oleg Kiselyov
\author Philippe Lavoie 
\date 22 Oct. 1997
*/
template <class T>
class SVDMatrix 
{
public:
	SVDMatrix():U(U_), V(V_), sig(sig_) { ; }
	SVDMatrix(const Matrix<T>& A);

	Matrix<T>& U ;
	Matrix<T>& V ;
	std::vector<T>& sig ;

	int decompose(const Matrix<T>& A) ;
	void minMax(T& min_sig, T& max_sig) const;
	double q_cond_number(void) const;	

	void cut_off(const double min_sig);
	void inverseIn(Matrix<T>& inv, double tau=0) ;
	Matrix<T> inverse(double tau=0) ;
	int solve(const Matrix<T>& B, Matrix<T>& X, double tau=0) ;

protected:
	int M,N;	//!< Dimensions of the problem (M > N)
	Matrix<T> U_;	  //!< M * M orthogonal matrix \a U
	Matrix<T> V_;	  //!< N * N orthogonal matrix \a V
	std::vector<T> sig_; //!< std::vector of \a N unordered singular values

	// Internal procedures used in SVD
	double left_householder(Matrix<T>& A, const int i);
	double right_householder(Matrix<T>& A, const int i);
	double bidiagonalize(std::vector<T>& super_diag, const Matrix<T>& _A);

	void rotate(Matrix<T>& U, const int i, const int j,
		const double cos_ph, const double sin_ph);
	void rip_through(std::vector<T>& super_diag, const int k, const int l, const double eps);
	int get_submatrix_to_work_on(std::vector<T>& super_diag, const int k, const double eps);
	void diagonalize(std::vector<T>& super_diag, const double eps);

};

/*
*------------------------------------------------------------------------
*				Bidiagonalization
*/

/*
*			Left Householder Transformations
*
* Zero out an entire subdiagonal of the i-th column of A and compute the
* modified A[i,i] by multiplication (UP' * A) with a matrix UP'
*   (1)  UP' = E - UPi * UPi' / beta
*
* where a column-vector UPi is as follows
*   (2)  UPi = [ (i-1) zeros, A[i,i] + Norm, vector A[i+1:M,i] ]
* where beta = UPi' * A[,i] and Norm is the norm of a vector A[i:M,i]
* (sub-diag part of the i-th column of A). Note we assign the Norm the
* same sign as that of A[i,i].
* By construction, (1) does not affect the first (i-1) rows of A. Since
* A[*,1:i-1] is bidiagonal (the result of the i-1 previous steps of
* the bidiag algorithm), transform (1) doesn't affect these i-1 columns
* either as one can easily verify.
* The i-th column of A is transformed as
*   (3)  UP' * A[*,i] = A[*,i] - UPi
* (since UPi'*A[*,i]/beta = 1 by construction of UPi and beta)
* This means effectively zeroing out A[i+1:M,i] (the entire subdiagonal
* of the i-th column of A) and replacing A[i,i] with the -Norm. Thus
* modified A[i,i] is returned by the present function.
* The other (i+1:N) columns of A are transformed as
*    (4)  UP' * A[,j] = A[,j] - UPi * ( UPi' * A[,j] / beta )
* Note, due to (2), only elements of rows i+1:M actually  participate
* in above transforms; the upper i-1 rows of A are not affected.
* As was mentioned earlier,
* (5)  beta = UPi' * A[,i] = (A[i,i] + Norm)*A[i,i] + A[i+1:M,i]*A[i+1:M,i]
*	= ||A[i:M,i]||^2 + Norm*A[i,i] = Norm^2 + Norm*A[i,i]
* (note the sign of the Norm is the same as A[i,i])
* For extra precision, vector UPi (and so is Norm and beta) are scaled,
* which would not affect (4) as easy to see.
*
* To satisfy the definition
*   (6)  .SIG = U' A V
* the result of consecutive transformations (1) over matrix A is accumulated
* in matrix U' (which is initialized to be a unit matrix). At each step,
* U' is left-multiplied by UP' = UP (UP is symmetric by construction,
* see (1)). That is, U is right-multiplied by UP, that is, rows of U are
* transformed similarly to columns of A, see eq. (4). We also keep in mind
* that multiplication by UP at the i-th step does not affect the first i-1
* columns of U.
* Note that the vector UPi doesn't have to be allocated explicitly: its
* first i-1 components are zeros (which we can always imply in computations),
* and the rest of the components (but the UPi[i]) are the same as those
* of A[i:M,i], the subdiagonal of A[,i]. This column, A[,i] is affected only
* trivially as explained above, that is, we don't need to carry this
* transformation explicitly (only A[i,i] is going to be non-trivially
* affected, that is, replaced by -Norm, but we will use sig[i] to store
* the result).
*
*/

template <class T> 
double SVDMatrix<T>::left_householder(Matrix<T>& A, const int i)
{
	int j,k;
	T scale = 0;			// Compute the scaling factor
	for(k=i; k<M; k++)
		scale += fabs(A(k,i));
	if( scale == 0 )			// If A[,i] is a null vector, no
		return 0;				// transform is required
	double Norm_sqr = 0;			// Scale UPi (that is, A[,i])
	for(k=i; k<M; k++)			// and compute its norm, Norm^2
	{
		A(k,i) /= scale;
		Norm_sqr += A(k,i)*A(k,i);
	}
	double new_Aii = sqrt(Norm_sqr);	// new_Aii = -Norm, Norm has the
	if( A(i,i) > 0 ) new_Aii = -new_Aii;	// same sign as Aii (that is, UPi[i])
	const float beta = - A(i,i)*new_Aii + Norm_sqr;
	A(i,i) -= new_Aii;			// UPi[i] = A[i,i] - (-Norm)

	for(j=i+1; j<N; j++)		// Transform i+1:N columns of A
	{
		T factor = 0;
		for(k=i; k<M; k++)
			factor += A(k,i) * A(k,j);	
		factor /= beta;
		for(k=i; k<M; k++)
			A(k,j) -= A(k,i) * factor;
	}

	for(j=0; j<M; j++)			// Accumulate the transform in U
	{
		T factor = 0;
		for(k=i; k<M; k++)
			factor += A(k,i) * U_(j,k);	// Compute  U[j,] * UPi
		factor /= beta;
		for(k=i; k<M; k++)
			U_(j,k) -= A(k,i) * factor;
	}
	return new_Aii * scale;		// Scale new Aii back (our new Sig[i])
}

/*
*			Right Householder Transformations
*
* Zero out i+2:N elements of a row A[i,] of matrix A by right
* multiplication (A * VP) with a matrix VP
*   (1)  VP = E - VPi * VPi' / beta
*
* where a vector-column .VPi is as follows
*   (2)  VPi = [ i zeros, A[i,i+1] + Norm, vector A[i,i+2:N] ]
* where beta = A[i,] * VPi and Norm is the norm of a vector A[i,i+1:N]
* (right-diag part of the i-th row of A). Note we assign the Norm the
* same sign as that of A[i,i+1].
* By construction, (1) does not affect the first i columns of A. Since
* A[1:i-1,] is bidiagonal (the result of the previous steps of
* the bidiag algorithm), transform (1) doesn't affect these i-1 rows
* either as one can easily verify.
* The i-th row of A is transformed as
*  (3)  A[i,*] * VP = A[i,*] - VPi'
* (since A[i,*]*VPi/beta = 1 by construction of VPi and beta)
* This means effectively zeroing out A[i,i+2:N] (the entire right super-
* diagonal of the i-th row of A, but ONE superdiag element) and replacing
* A[i,i+1] with - Norm. Thus modified A[i,i+1] is returned as the result of
* the present function.
* The other (i+1:M) rows of A are transformed as
*    (4)  A[j,] * VP = A[j,] - VPi' * ( A[j,] * VPi / beta )
* Note, due to (2), only elements of columns i+1:N actually  participate
* in above transforms; the left i columns of A are not affected.
* As was mentioned earlier,
* (5)  beta = A[i,] * VPi = (A[i,i+1] + Norm)*A[i,i+1]
*			   + A[i,i+2:N]*A[i,i+2:N]
*	= ||A[i,i+1:N]||^2 + Norm*A[i,i+1] = Norm^2 + Norm*A[i,i+1]
* (note the sign of the Norm is the same as A[i,i+1])
* For extra precision, vector VPi (and so is Norm and beta) are scaled,
* which would not affect (4) as easy to see.
*
* The result of consecutive transformations (1) over matrix A is accumulated
* in matrix V (which is initialized to be a unit matrix). At each step,
* V is right-multiplied by VP. That is, rows of V are transformed similarly
* to rows of A, see eq. (4). We also keep in mind that multiplication by
* VP at the i-th step does not affect the first i rows of V.
* Note that vector VPi doesn't have to be allocated explicitly: its
* first i components are zeros (which we can always imply in computations),
* and the rest of the components (but the VPi[i+1]) are the same as those
* of A[i,i+1:N], the superdiagonal of A[i,]. This row, A[i,] is affected
* only trivially as explained above, that is, we don't need to carry this
* transformation explicitly (only A[i,i+1] is going to be non-trivially
* affected, that is, replaced by -Norm, but we will use super_diag[i+1] to
* store the result).
*
*/

template <class T>
double SVDMatrix<T>::right_householder(Matrix<T>& A, const int i)
{
	int j,k;
	T scale = 0;			// Compute the scaling factor
	for(k=i+1; k<N; k++)
		scale += fabs(A(i,k));
	if( scale == 0 )			// If A[i,] is a null vector, no
		return 0;				// transform is required

	double Norm_sqr = 0;			// Scale VPi (that is, A[i,])
	for(k=i+1; k<N; k++)		// and compute its norm, Norm^2
	{
		A(i,k) /= scale;
		Norm_sqr += A(i,k)*A(i,k);
	}
	double new_Aii1 = sqrt(Norm_sqr);	// new_Aii1 = -Norm, Norm has the
	if( A(i,i+1) > 0 )			// same sign as
		new_Aii1 = -new_Aii1; 		// Aii1 (that is, VPi[i+1])
	const float beta = - A(i,i+1)*new_Aii1 + Norm_sqr;
	A(i,i+1) -= new_Aii1;		// VPi[i+1] = A[i,i+1] - (-Norm)

	for(j=i+1; j<M; j++)		// Transform i+1:M rows of A
	{
		T factor = 0;
		for(k=i+1; k<N; k++)
			factor += A(i,k) * A(j,k);	// Compute A[j,] * VPi
		factor /= beta;
		for(k=i+1; k<N; k++)
			A(j,k) -= A(i,k) * factor;
	}

	for(j=0; j<N; j++)			// Accumulate the transform in V
	{
		T factor = 0;
		for(k=i+1; k<N; k++)
			factor += A(i,k) * V_(j,k);	// Compute  V[j,] * VPi
		factor /= beta;
		for(k=i+1; k<N; k++)
			V_(j,k) -= A(i,k) * factor;
	}
	return new_Aii1 * scale;		// Scale new Aii1 back
}

/*
*------------------------------------------------------------------------
*			  Bidiagonalization
* This nethod turns matrix A into a bidiagonal one. Its N diagonal elements
* are stored in a vector sig, while N-1 superdiagonal elements are stored
* in a vector super_diag(2:N) (with super_diag(1) being always 0).
* Matrices U and V store the record of orthogonal Householder
* reflections that were used to convert A to this form. The method
* returns the norm of the resulting bidiagonal matrix, that is, the
* maximal column sum.
*/

template <class T>
double SVDMatrix<T>::bidiagonalize(std::vector<T>& super_diag, const Matrix<T>& _A)
{
	double norm_acc = 0;
	super_diag[0] = 0;			// No superdiag elem above A(1,1)
	Matrix<T> A = _A;			// A being transformed
	for(int i=0; i<N; i++)
	{
		const T& diagi = sig_[i] = left_householder(A,i);
		if( i < N-1 )
			super_diag[i+1] = right_householder(A,i);
		double t2 = fabs(diagi)+fabs(super_diag[i]) ;
		norm_acc = max(norm_acc,t2);
	}
	return norm_acc;
}

/*
*------------------------------------------------------------------------
*		QR-diagonalization of a bidiagonal matrix
*
* After bidiagonalization we get a bidiagonal matrix J:
*    (1)  J = U' * A * V
* The present method turns J into a matrix JJ by applying a set of
* orthogonal transforms
*    (2)  JJ = S' * J * T
* Orthogonal matrices S and T are chosen so that JJ were also a
* bidiagonal matrix, but with superdiag elements smaller than those of J.
* We repeat (2) until non-diag elements of JJ become smaller than EPS
* and can be disregarded.
* Matrices S and T are constructed as
*    (3)  S = S1 * S2 * S3 ... Sn, and similarly T
* where Sk and Tk are matrices of simple rotations
*    (4)  Sk[i,j] = i==j ? 1 : 0 for all i>k or i<k-1
*         Sk[k-1,k-1] = cos(Phk),  Sk[k-1,k] = -sin(Phk),
*         SK[k,k-1] = sin(Phk),    Sk[k,k] = cos(Phk), k=2..N
* Matrix Tk is constructed similarly, only with angle Thk rather than Phk.
*
* Thus left multiplication of J by SK' can be spelled out as
*    (5)  (Sk' * J)[i,j] = J[i,j] when i>k or i<k-1,
*                  [k-1,j] = cos(Phk)*J[k-1,j] + sin(Phk)*J[k,j]
*                  [k,j] =  -sin(Phk)*J[k-1,j] + cos(Phk)*J[k,j]
* That is, k-1 and k rows of J are replaced by their linear combinations;
* the rest of J is unaffected. Right multiplication of J by Tk similarly
* changes only k-1 and k columns of J.
* Matrix T2 is chosen the way that T2'J'JT2 were a QR-transform with a
* shift. Note that multiplying J by T2 gives rise to a J[2,1] element of
* the product J (which is below the main diagonal). Angle Ph2 is then
* chosen so that multiplication by S2' (which combines 1 and 2 rows of J)
* gets rid of that elemnent. But this will create a [1,3] non-zero element.
* T3 is made to make it disappear, but this leads to [3,2], etc.
* In the end, Sn removes a [N,N-1] element of J and matrix S'JT becomes
* bidiagonal again. However, because of a special choice
* of T2 (QR-algorithm), its non-diag elements are smaller than those of J.
*
* All this process in more detail is described in
*    J.H. Wilkinson, C. Reinsch. Linear algebra - Springer-Verlag, 1971
*
* If during transforms (1), JJ[N-1,N] turns 0, then JJ[N,N] is a singular
* number (possibly with a wrong (that is, negative) sign). This is a
* consequence of Frantsis' Theorem, see the book above. In that case, we can
* eliminate the N-th row and column of JJ and carry out further transforms
* with a smaller matrix. If any other superdiag element of JJ turns 0,
* then JJ effectively falls into two independent matrices. We will process
* them independently (the bottom one first).
*
* Since matrix J is a bidiagonal, it can be stored efficiently. As a matter
* of fact, its N diagonal elements are in array Sig, and superdiag elements
* are stored in array super_diag.
*/

// Carry out U * S with a rotation matrix
// S (which combines i-th and j-th columns
// of U, i>j)
template <class T>
void SVDMatrix<T>::rotate(Matrix<T>& Mu, const int i, const int j,
						  const double cos_ph, const double sin_ph)
{
	for(int l=0; l<Mu.sizeRows(); l++)
	{
		T& Uil = Mu(l,i); T& Ujl = Mu(l,j);
		const T Ujl_was = Ujl;
		Ujl =  cos_ph*Ujl_was + sin_ph*Uil;
		Uil = -sin_ph*Ujl_was + cos_ph*Uil;
	}
}
/*
* A diagonal element J[l-1,l-1] turns out 0 at the k-th step of the
* algorithm. That means that one of the original matrix' singular numbers
* shall be zero. In that case, we multiply J by specially selected
* matrices S' of simple rotations to eliminate a superdiag element J[l-1,l].
* After that, matrix J falls into two pieces, which can be dealt with
* in a regular way (the bottom piece first).
* 
* These special S transformations are accumulated into matrix U: since J
* is left-multiplied by S', U would be right-multiplied by S. Transform
* formulas for doing these rotations are similar to (5) above. See the
* book cited above for more details.
*/
template <class T>
void SVDMatrix<T>::rip_through(std::vector<T>& super_diag, const int k, const int l, const double eps)
{
	double cos_ph = 0, sin_ph = 1;	// Accumulate cos,sin of Ph
	// The first step of the loop below
	// (when i==l) would eliminate J[l-1,l],
	// which is stored in super_diag(l)
	// However, it gives rise to J[l-1,l+1]
	// and J[l,l+2]
	// The following steps eliminate these
	// until they fall below
	// significance
	for(int i=l; i<=k; i++)
	{
		const double f = sin_ph * super_diag[i];
		super_diag[i] *= cos_ph;
		if( fabs(f) <= eps )
			break;			// Current J[l-1,l] became unsignificant
		cos_ph = sig[i]; sin_ph = -f;	// unnormalized sin/cos
		const double norm = (sig_[i] = hypot(cos_ph,sin_ph)); // sqrt(sin^2+cos^2)
		cos_ph /= norm, sin_ph /= norm;	// Normalize sin/cos
		rotate(U_,i,l-1,cos_ph,sin_ph);
	}
}

// We're about to proceed doing QR-transforms
// on a (bidiag) matrix J[1:k,1:k]. It may happen
// though that the matrix splits (or can be
// split) into two independent pieces. This function
// checks for splitting and returns the lowerbound
// index l of the bottom piece, J[l:k,l:k]
template <class T>
int SVDMatrix<T>::get_submatrix_to_work_on(std::vector<T>& super_diag, const int k, const double eps)
{
	for(register int l=k; l>0; l--)
		if( fabs(super_diag[l]) <= eps )
			return l;				// The breaking point: zero J[l-1,l]
		else if( fabs(sig[l-1]) <= eps )	// Diagonal J[l,l] turns out 0
		{					// meaning J[l-1,l] _can_ be made
			rip_through(super_diag,k,l,eps);	// zero after some rotations
			return l;
		}
		return 0;			// Deal with J[1:k,1:k] as a whole
}

// Diagonalize root module
template <class T>
void SVDMatrix<T>::diagonalize(std::vector<T>& super_diag, const double eps)
{
	for(register int k=N-1; k>=0; k--)	// QR-iterate upon J[l:k,l:k]
	{
		int l;
		//    while(l=get_submatrix_to_work_on(super_diag,k,eps),
		//	  fabs(super_diag[k]) > eps)	// until superdiag J[k-1,k] becomes 0
		// Phil
		while(fabs(super_diag[k]) > eps)// until superdiag J[k-1,k] becomes 0
		{
			l=get_submatrix_to_work_on(super_diag,k,eps) ;
			double shift;			// Compute a QR-shift from a bottom
			{					// corner minor of J[l:k,l:k] order 2
				T Jk2k1 = super_diag[k-1],	// J[k-2,k-1]
					Jk1k  = super_diag[k],
					Jk1k1 = sig[k-1],		// J[k-1,k-1]
					Jkk   = sig[k],
					Jll   = sig[l];		// J[l,l]
				shift = (Jk1k1-Jkk)*(Jk1k1+Jkk) + (Jk2k1-Jk1k)*(Jk2k1+Jk1k);
				shift /= 2*Jk1k*Jk1k1;
				shift += (shift < 0 ? -1 : 1) * sqrt(shift*shift+1);
				shift = ( (Jll-Jkk)*(Jll+Jkk) + Jk1k*(Jk1k1/shift-Jk1k) )/Jll;
			}
			// Carry on multiplications by T2, S2, T3...
			double cos_th = 1, sin_th = 1;
			T Ji1i1 = sig[l];	// J[i-1,i-1] at i=l+1...k
			for(register int i=l+1; i<=k; i++)
			{
				T Ji1i = super_diag[i], Jii = sig[i];  // J[i-1,i] and J[i,i]
				sin_th *= Ji1i; Ji1i *= cos_th; cos_th = shift;
				double norm_f = (super_diag[i-1] = hypot(cos_th,sin_th));
				cos_th /= norm_f, sin_th /= norm_f;
				// Rotate J[i-1:i,i-1:i] by Ti
				shift = cos_th*Ji1i1 + sin_th*Ji1i;	// new J[i-1,i-1]
				Ji1i = -sin_th*Ji1i1 + cos_th*Ji1i;	// J[i-1,i] after rotation
				const double Jii1 = Jii*sin_th;		// Emerged J[i,i-1]
				Jii *= cos_th;				// new J[i,i]
				rotate(V_,i,i-1,cos_th,sin_th); // Accumulate T rotations in V

				double cos_ph = shift, sin_ph = Jii1;// Make Si to get rid of J[i,i-1]
				sig_[i-1] = (norm_f = hypot(cos_ph,sin_ph));	// New J[i-1,i-1]
				if( norm_f == 0 )		// If norm =0, rotation angle
					cos_ph = cos_th, sin_ph = sin_th; // can be anything now
				else
					cos_ph /= norm_f, sin_ph /= norm_f;
				// Rotate J[i-1:i,i-1:i] by Si
				shift = cos_ph * Ji1i + sin_ph*Jii;	// New J[i-1,i]
				Ji1i1 = -sin_ph*Ji1i + cos_ph*Jii;	// New Jii, would carry over
				// as J[i-1,i-1] for next i
				rotate(U_,i,i-1,cos_ph,sin_ph);  // Accumulate S rotations in U
				// Jii1 disappears, sin_th would
				cos_th = cos_ph, sin_th = sin_ph; // carry over a (scaled) J[i-1,i+1]
				// to eliminate on the next i, cos_th
				// would carry over a scaled J[i,i+1]
			}
			super_diag[l] = 0;		// Supposed to be eliminated by now
			super_diag[k] = shift;
			sig_[k] = Ji1i1;
		}		// --- end-of-QR-iterations
		if( sig[k] < 0 )		// Correct the sign of the sing number
		{
			sig_[k] = -sig_[k];
			for(register int j=0; j<V_.sizeRows(); j++)
				V_(j,k) = -V_(j,k);
		}
	}
} 

/*!
\brief Performs the SVD decomposition of A

Performs the SVD decomposition of A. It is not recommended
to use this routine has errors from the decomposition 
routine are discarded

\param A  the matrix to decompose

\warning The matrix A must have a number equal or higher of rows than 
columns.

\author Philippe Lavoie 
\date 22 October 1997
*/
template <class T>
SVDMatrix<T>::SVDMatrix(const Matrix<T>& A)
: U(U_),V(V_),sig(sig_)
{
	decompose(A) ;
}

/*!
\brief Performs the SVD decomposition of A

Performs the SVD decomposition of A.

\param A  the matrix to decompose
\return 1 if the decomposition was succesfull, 0 otherwise.

\warning The matrix A must have a number equal or higher of rows than 
columns.

\author Philippe Lavoie 
\date 22 October 1997
*/
template <class T>
int SVDMatrix<T>::decompose(const Matrix<T>& A){
	M = A.sizeRows() ;
	N = A.sizeCols() ;

	if( M < N ){
		return 0 ;
	}

	U_.resize(M,M) ;
	V_.resize(N,N) ;
	sig_.resize(N) ;

	U_.setDiagonal(1) ;
	V_.setDiagonal(1) ;

	std::vector<T> super_diag(N);
	const double bidiag_norm = bidiagonalize(super_diag,A);
	const double eps = FLT_EPSILON * bidiag_norm;	// Significance threshold
	diagonalize(super_diag,eps);

	return 1 ;
}


/*!
\brief Finds the minimal and maximal sigma values

Finds the minimal and maximal sigma values.

\param min_sig  the minimal sigma value
\param max_sig  the maximal sigma value

\warning The SVD matrix must be valid.

\author Philippe Lavoie 
\date 22 October 1997
*/
template <class T>
void SVDMatrix<T>::minMax(T& min_sig, T& max_sig) const
{
	max_sig = min_sig = sig[0];
	for(register int i=0; i<sig.size(); ++i)
		if( sig[i] > max_sig )
			max_sig = sig[i];
		else if( sig[i] < min_sig )
			min_sig = sig[i];
}

/*!
\brief Finds the condition number

Finds the condition number which corresponds to the maximal
sigma value divided by its minimal value.

\return The condition number

\warning The SVD matrix must be valid.

\author Philippe Lavoie 
\date 22 October 1997
*/
template <class T>
double SVDMatrix<T>::q_cond_number(void) const
{
	T mins,maxs ;
	minMax(mins,maxs) ;
	return (maxs/mins);
}

/*!
\brief Finds the (pseudo-)inverse of a SVD matrix

Finds the (pseudo-)inverse of a SVD matrix.

\param   A  the inverse of the SVD matrix
\param tau  the minimal singular value accepted

\warning The SVD matrix must be valid.

\author Philippe Lavoie 
\date 22 October 1997
*/
template <class T>
void SVDMatrix<T>::inverseIn(Matrix<T>& A, double tau) {
	Matrix<T> S ;

	T min_sig, max_sig ;
	minMax(min_sig,max_sig) ;

	if(tau==0)
		tau = V.sizeRows()*max_sig*FLT_EPSILON ;

	if(min_sig<tau){
		return ;
	}

	S.resize(V.sizeCols(),U.sizeRows());
	S.reset(0) ;
	for(int i=0;i<sig.n();++i)
		S(i,i) = (T)1/sig[i] ;

	//A = V*S*transpose(U) ; 
	A = U.transpose() ;
	A = (const Matrix<T>&)S*(const Matrix<T>&)A ; // transpose(U) ; 
	A = (const Matrix<T>&)V*(const Matrix<T>&)A ;
}


/*!
\brief Finds the (pseudo-)inverse of a SVD matrix

Finds the (pseudo-)inverse of a SVD matrix.

\param tau  the minimal singular value accepted

\warning The SVD matrix must be valid.

\author Philippe Lavoie 
\date 22 October 1997
*/
template <class T>
Matrix<T> SVDMatrix<T>::inverse(double tau) {
	Matrix<T> A ;

	inverseIn(A,tau) ;
	return A ;
}

/*!
\brief Solves the linear system \a A \a X = \a B

Solves the linear system \a A \a X = \a B. Given \a A and \a B it
finds the value of \a X. \a A is the SVD matrix properly
initialized and the only other input parameter is \a B.

\param B  the right hand side of the equation
\param X  the resulting matrix
\param tau  the minimal singular value accepted

\return 1 if the routine was able to solve the problem, 0 otherwise.

\warning The SVD matrix must be valid and correspond to \a A.

\author Philippe Lavoie 
\date 22 October 1997
*/
template <class T>
int SVDMatrix<T>::solve(const Matrix<T>&BB, Matrix<T>&X, double tau) {
	 Matrix<T>& B =  (Matrix<T>&)BB;
	T min_sig, max_sig ;
	minMax(min_sig,max_sig) ;

	if(B.sizeRows() != U.sizeRows()){
		return 0 ;
	}

	X.resize(V.sizeRows(),B.sizeCols()) ;

	if(tau==0)
		tau = V.sizeRows()*max_sig*FLT_EPSILON ;
	int stable = 1 ;

	std::vector<T> S(sig.size()) ;

	// doing one column from B at a time 
	int i,j,k ;
	for(j=0;j<B.sizeCols();++j){
		for(i=0;i<V.sizeCols();++i){
			T s = 0 ;
			if(sig[i]>tau){
				for(k=0;k<U.sizeCols();++k)
					s += U(k,i)*B(k,j);
				s /= sig[i] ;
			}
			else
				stable = 0 ;
			S[i] = s ;
		}
		for(i=0;i<V.sizeRows();++i){
			X(i,j) = 0 ;
			for(k=0;k<V.sizeRows();++k)
				X(i,j) += V(i,k)*S[k] ;
		}
	}
	return stable ;
}

/*!
\brief Solves the linear system \a A \a X = \a B

Solves the linear system \a A \a X = \a B. Given \a A and B it
finds the value of \a X. The routine uses LU decomposition
if the A matrix is square and it uses SVD decomposition
otherwise.

\param   A  the \a A matrix
\param B  the right hand side of the equation
\param X  the resulting matrix

\return 1 if the SVD decomposition worked.

\author Philippe Lavoie 
\date 22 October 1997
*/
template <class T> 
int solve(const Matrix<T>& A, 
		  const Matrix<T>& B, 
		  Matrix<T>& X){
			  if(A.sizeRows()==A.sizeCols()){ 
				  // use LU decomposition to solve the problem
				  LUMatrix<T> lu(A) ;
				  X = lu.inverse()*B ;
			  }
			  else{
				  SVDMatrix<T> svd(A) ;
				  return svd.solve(B,X) ;
			  }
			  return 1;
}

/*!
\brief finds the inverse of a matrix

Finds the inverse of a matrix. It uses LU decomposition if
the matrix is square and it uses SVD decomposition otherwise.

\param A  the matrix to inverse

\return The inverse of A

\warning The A matrix must have a number of rows greater or equal to 
its number of columns.

\author Philippe Lavoie 
\date 22 October 1997
*/
template <class T>
Matrix<T> inverse(const Matrix<T>&A){
	Matrix<T> inv ;
	if(A.sizeRows()==A.sizeCols()){
		LUMatrix<T> lu(A) ;
		lu.inverseIn(inv) ;
	}
	else{
		SVDMatrix<T> svd(A) ;
		svd.inverseIn(inv) ;
	}
	return inv ;
}

end_gtl_namespace