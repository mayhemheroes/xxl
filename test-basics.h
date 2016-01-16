	VP a,b,c,d; int i, tc;

	for(tc=0;tc<TESTITERS;tc++) {
		a=xalloc(T_i,1);
		EL(a,int,0)=1;
		ASSERT(EL(a,int,0)==1,"x0");
		ASSERT(AS_i(a,0)==1,"AS_i0");
		xfree(a);
		a=xi(99);
		b=xi(99);
		ASSERT(EL(a,int,0)==99,"x1");
		ASSERT(_equal(a,b)==1,"_equal0");
		ASSERT(_equal(b,a)==1,"_equal1");
		ASSERT(_equal(a,a)==1,"_equal2");
		ASSERT(_equal(b,b)==1,"_equal3");
		a=append(a,xi(98));
		ASSERT(EL(a,int,0)==99,"x2");
		ASSERT(EL(a,int,1)==98,"x3");
		ASSERT(_equal(a,b)==0,"_equal4");
		ASSERT(_equal(b,a)==0,"_equal5");
		c=xi(0);
		PFW({
		DUMP(apply(a,c));
		DUMP(b);
		});
		ASSERT(_equal(apply(a,c),b)==1,"apply eq 0");
		xfree(b); 
		xfree(c);
		b=xi(98);
		ASSERT(_contains(a,b)==1,"_contains 0");
		c=xi(10);
		ASSERT(_contains(a,c)==0,"_contains 1");
		ASSERT(AS_i(contains(a,b),0)==1,"contains 0");
		ASSERT(AS_i(contains(a,c),0)==0,"contains 1");
		xfree(b);
		xfree(a);
		xfree(c);
		ASSERT(!_equal(xi(-1),xi(1)),"xi neg 0");
		a=xin(5,1,2,3,4,100);
		ASSERT(a->n==5,"_xin 0");
		ASSERT(EL(a,int,4)==100,"_xin 1");
		a=xian(a,3,200,300,400);
		ASSERT(EL(a,int,4)==100,"xian 2");
		ASSERT(EL(a,int,7)==400,"xian 3");
		ASSERT(EL(a,int,0)==1,"xian 4");
		ASSERT(a->n==8,"xian 5");
		#define NADD 1024
		for(i=0;i<NADD;i++) {
			a=xia(a,i*3);
		}
		ASSERT(a->n==8+NADD,"xia 1");
		ASSERT(EL(a,int,0)==1,"xia 1b");
		ASSERT(EL(a,int,1)==2,"xia 1c");
		ASSERT(EL(a,int,NADD+7)==(NADD-1)*3,"xia 2");
		xfree(a);
		a=xl0();
		b=xi(100);
		append(a,b);
		ASSERT(a->n=1,"append i 0");
		xfree(b);
		b = ELl(a,0);
		ASSERT(b->n==1,"append i 1");
		ASSERT(EL(b,int,0)==100,"append i 2");
		append(a,xfroms("test"));
		append(a,xfroms("test2"));
		DUMP(a);
		ASSERT(a->n==3,"append str 1");
		ASSERT(ELl(a,1)->n==5,"append str 2"); // test\0
		ASSERT(ELl(a,2)->n==6,"append str 2"); // test2\0
		ASSERT(_contains(a,ELl(a,2))==1,"str contains 0");
		ASSERT(_contains(a,xfroms("tes"))==0,"str contains 1");
		ASSERT(_equal(take(xin(3,1,2,3),xi(1)),xi(1)),"take 0");
		ASSERT(_equal(take(xin(3,1,2,3),xi(2)),xin(2,1,2)),"take 1");
		PFW({
		ASSERT(_equal(take(xin(3,1,2,3),xi(-2)),xin(2,2,3)),"take 2");
		ASSERT(_equal(take(xin(3,1,2,3),xi(-1)),xi(3)),"take 3");
		ASSERT(_equal(take(xin(3,1,2,3),xi(0)),xi0()),"take 4");
		ASSERT(_equal(take(xin(3,1,2,3),xi(4)),xi0()),"take 5");
		ASSERT(_equal(take(xin(3,1,2,3),xi(3)),xin(3,1,2,3)),"take 5");
		});
		upsert(a,xfroms("test"));
		ASSERT(a->n==3,"upsert 1");
		b = xi(101);
		upsert(a,b);
		xfree(b);
		DUMP(a);
		ASSERT(a->n==4,"upsert 2");
		ASSERT(_find(a,xfroms("test"))==1,"_find 0");
		ASSERT(_find(a,xfroms("est"))==-1,"_find 1");
		ASSERT(_find(a,xfroms("tes"))==-1,"_find 2");
		xfree(a);
		PFW({
		a = xd0();
		b = apply(a,xfroms("a"));
		DUMP(a);
		DUMP(b);
		ASSERT(b==NULL,"apply 0");
		});
		b = xln(2,xi(1),xi(10)); // key:value dict
		append(a,b);
		xfree(b);
		d = xi(1);
		c = apply(a,d);
		ASSERT(_equal(c,xi(10))==1,"apply equal 0");
		xfree(a); xfree(c); xfree(d);
		a = xd0();
		b = xln(2,xln(1,xfroms("name")),xfroms("tom"));
		a=append(a,b);
		c=apply(a,xln(1,xfroms("name")));
		DUMP(c);
		ASSERT(IS_c(c) && memcmp(BUF(c),"tom",c->n)==0,"apply equal str");
		xfree(c);
		c=apply(a,xln(1,xfroms("")));
		ASSERT(c->n==0,"apply empty search");
		DUMP(c);
		xfree(c);
		xfree(b);
		xfree(a);
		_tagnums("");
		a=xin(3,10,9,8);
		a->tag=_tagnums("test tag");
		ASSERT(strcmp(sfromx(tagname(a->tag)),"test tag")==0,"tag name 0");
		xfree(a);

		a=xin(3,1,2,3); b=cast(a,xt(Ti(byte)));
		DUMP(b);
		ASSERT(!_equal(a,b)&&b->n==3&&AS_b(b,2)==3,"cib");
		xfree(a);xfree(b);
		a=xin(3,1,2,3); b=cast(a,xt(Ti(octo)));
		DUMP(b);
		ASSERT(!_equal(a,b)&&b->n==3&&AS_o(b,2)==3,"cio");
		xfree(a);
		xfree(b);
		a=xbn(2,9,8); b=cast(a,xt(Ti(byte)));
		DUMP(b);
		ASSERT(_equal(a,b)&&b->n==2&&AS_b(b,0)==9,"cbb");
		xfree(a);xfree(b);
	}
