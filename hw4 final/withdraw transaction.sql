delimiter //
create procedure Withdraw (c char(20),id char(20), out output char(20))
begin 
declare gd char(20);
declare yr int;
declare qt char(2);
declare t_error int default 0;
declare continue handler for sqlexception set t_error=1;

start transaction;
if( c in (select UoSCode
          from transcript
          where StudId=id))
then
begin 
set gd=(select grade
        from transcript
        where StudId=id and UoSCode=c);
if((gd='CR')or(gd='D')or(gd='P'))
then set output= 'hasgrade';
else 
  begin
   set qt=(select Semester
           from transcript
           where UoSCode=c and StudId=id);
   set yr=(select year
		   from transcript
		   where UoSCode=c and StudId=id);
   
   delete from transcript
   where UoSCode=c and StudId=id;
   
   update uosoffering
   set Enrollment=Enrollment-1
   where UoSCode=c and year=yr and Semester=qt;
   
   set output= 'withdraw';
   end;
   
end if;
end;
else set output= 'notFoundcourse';
end if;
if t_error=1 then begin
set output='ERROR, rollback';
select output;
rollback;
end;
else begin 
select output;
commit;
end;
end if;
end //
delimiter ;
