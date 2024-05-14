echo "declare @va_NumSortieAmont_out			tinyint
declare @va_DistanceSortieAmont_out		int
declare @va_NumSortieAval_out			tinyint
declare @va_DistanceSortieAval_out		int
declare @va_NumPeageAmont_out			tinyint
declare @va_DistancePeageAmont_out		int
declare @va_NumPeageAval_out			tinyint
declare @va_DistancePeageAval_out		int
declare @va_LongueurPlateforme_out		int
declare @va_NumAutorouteBifAmont_out		tinyint
declare @va_DistanceBifAmont_out		int
declare @va_NumAutorouteBifAval_out		tinyint
declare @va_DistanceBifAval_out			int
declare @va_Descente_out			char(25)
declare @va_Tunnel_out				char(25)
declare @va_NomSortieAm_out			char(25)
declare @va_AbrevSortieAm_out			char(15)
declare @va_NomSortieAv_out			char(25)
declare @va_AbrevSortieAv_out			char(15)
declare @va_NomPeageAm_out			char(25)
declare @va_AbrevPeageAm_out			char(15)
declare @va_NomPeageAval_out			char(25)
declare @va_AbrevPeageAval_out			char(15)
declare @va_Pente_out				smallint
declare @va_Site_out				tinyint
declare @va_PrBifAmont_out			int
declare @va_PrBifAval_out			int
declare @va_PrDebut_BifAmont_out		int
declare @va_PrDebut_BifAval_out			int

exec PRC..XZAE;21 $1,
$2,
$3,
@va_NumSortieAmont_out output,
@va_DistanceSortieAmont_out output,
@va_NumSortieAval_out	output,
@va_DistanceSortieAval_out	 output,
@va_NumPeageAmont_out		 output,
@va_DistancePeageAmont_out	 output,
@va_NumPeageAval_out		 output,
@va_DistancePeageAval_out	 output,
@va_LongueurPlateforme_out	 output,
@va_NumAutorouteBifAmont_out	 output,
@va_DistanceBifAmont_out	 output,
@va_NumAutorouteBifAval_out	 output,
@va_DistanceBifAval_out	 output,
@va_Descente_out		 output,
@va_Tunnel_out			 output,
@va_NomSortieAm_out		 output,
@va_AbrevSortieAm_out		 output,
@va_NomSortieAv_out		 output,
@va_AbrevSortieAv_out		 output,
@va_NomPeageAm_out		 output,
@va_AbrevPeageAm_out		 output,
@va_NomPeageAval_out		 output,
@va_AbrevPeageAval_out		 output,
@va_Pente_out			 output,
@va_Site_out			 output,
@va_PrBifAmont_out		 output,
@va_PrBifAval_out		 output,
@va_PrDebut_BifAmont_out	 output,
@va_PrDebut_BifAval_out	 output
go" | isql -Usa -Pzigramur -SSQL_CI
