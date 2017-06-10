delimiter //
create procedure Enrollcourse12 (c char(8),qt char(2),yr char(20), id char(20), out output char(20))
begin
declare countpre int;
declare countprepass int;
declare enrollmentnum int;
declare maxnum int;

if (c in (select UoSCode
          from transcript
          where StudId=id))
then set output= 'failed';
else
begin
set maxnum=(select MaxEnrollment
            from uosoffering
            where UoSCode=c and Year=yr and Semester=qt);

set enrollmentnum=(select Enrollment
                   from uosoffering
                   where UoSCode=c and Year=yr and Semester=qt);
                   
set countprepass=(select count(UoSCode)
                  from transcript natural join 
                       (select PrereqUoSCode as UoSCode
                        from requires
					    where UoSCode=c) A
                        where ((grade='CR' or grade='P') and (studid=id)));
                     
set countpre=(select count(PrereqUoSCode)
              from requires
              where UoSCode=c);
if (countpre=countprepass) 
then if(enrollmentnum<maxnum)
  then 
   begin
        update uosoffering
        set Enrollment=Enrollment+1
        where UoSCode=c;
        
        insert into transcript values(id,c,qt,yr,null);
        set output= 'course enrolled'; 
     end;
    else set output= 'failed';
    end if;
else
     set output= 'lackpre';
     end if;
     end;
     end if;
     select output;
     end//
delimiter ;
