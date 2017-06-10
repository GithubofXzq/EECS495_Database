drop trigger if exists halfenroll;
drop table if exists flag;
create table flag(signal0 int);
delimiter \\
create 
	trigger halfenroll
    before update
    on uosoffering for each row
    begin
		if new.enrollment<old.MaxEnrollment*0.5 then
           insert into flag values(1);
		else delete from flag where signal0=1;
        end if;
	end\\
delimiter ;