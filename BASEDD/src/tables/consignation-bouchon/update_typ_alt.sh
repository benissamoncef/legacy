echo "
use CFG
go
insert into dbo.TYP_ALT (numero, nom) values (177, 'DOUBLON POTENTIEL PIT BOU')
go" | isql -Usa -Pzigramur -SSQL_CI
