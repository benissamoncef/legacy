isql -Usa -Pzigramur -SSQL_CI << !
use CFT
go
sp_dropkey primary, OPR_IDE
go
execute sp_primarykey OPR_IDE, email,login
go
drop index OPR_IDE.OPR_IDE_PK
go
create unique index OPR_IDE_PK on OPR_IDE (email,login)
go
use CFG
go
sp_dropkey primary, OPR_IDE
go
execute sp_primarykey OPR_IDE, email,login
go
drop index OPR_IDE.OPR_IDE_PK
go
create unique index OPR_IDE_PK on OPR_IDE (email,login)
go
!

isql -Usa -Pzigramur -SSQL_DS << !
use CFG
go
sp_dropkey primary, OPR_IDE
go
execute sp_primarykey OPR_IDE, email,login
go
drop index OPR_IDE.OPR_IDE_PK
go
create unique index OPR_IDE_PK on OPR_IDE (email,login)
go
!

isql -Usa -Pzigramur -SSQL_DM << !
use CFG
go
sp_dropkey primary, OPR_IDE
go
execute sp_primarykey OPR_IDE, email,login
go
drop index OPR_IDE.OPR_IDE_PK
go
create unique index OPR_IDE_PK on OPR_IDE (email,login)
go
!

isql -Usa -Pzigramur -SSQL_HIS << !
use CFG
go
sp_dropkey primary, OPR_IDE
go
execute sp_primarykey OPR_IDE, email,login
go
drop index OPR_IDE.OPR_IDE_PK
go
create unique index OPR_IDE_PK on OPR_IDE (email,login)
go
!
