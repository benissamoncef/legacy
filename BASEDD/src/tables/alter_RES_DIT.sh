echo "use CFT
go
alter table RES_DIT add version_radt char(10) default "1.0" not null
go" | sql ci
