/*E*/
/*Fichier : $Id: xzag100sp.prc,v 1.3 2007/03/26 15:25:08 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2007/03/26 15:25:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAG * FICHIER xzag100sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Rempli le champ PR_fin de la table RES_SEG
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAG;100
	@va_Basedd_in                char(3)
as 

/* 
*
* SP	XZAG100SP
*
* PARAMETRES EN ENTREE :
* le nom de la base			
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Rempli le champ PR_fin de la table RES_SEG
*
* MECANISMES :
*
---------------------------------------------------------------------
* HISTORIQUE :
*
* VATHELOT	28 Fev 1997	: Creation a partir de xzao214sp.prc
* VATHELOT	04 Avr 1997	: Ajout du nom de la base en parametre
----------------------------------------------------------------------*/
declare
   
/*A
** Declaration des variables locales
*/
   
@vl_Portion	smallint,
@vl_PR_debut	int,
@vl_PRf		int,
@vl_Sens	tinyint
	 
   
/*A
** Creation d'une table temporaire TMP_SEG pour retorner la liste des segments
*/
         
create table #TMP_SEG
(
portion		smallint	not null,
PR_debut	int		not null,
sens		tinyint		not null 
)
           
           
create table #TMP_SEG_RES
(
portion		smallint	not null,
PR_debut	int		not null,
PR_fin		int		not null,
sens		tinyint		not null 
)

    
if @va_Basedd_in = null
	return XDC_ARG_INV 


/* Travail dans CFG */

if @va_Basedd_in = XDC_BASE_CFG
begin

           
	/*A
	** Declaration d'un curseur sur la table RES_SEG
	*/
	
	insert #TMP_SEG
	select  
		CFG..RES_SEG.portion, 
		CFG..RES_SEG.PR_debut, 
		CFG..RES_SEG.sens
	from CFG..RES_SEG
         
	declare Segment cursor 
	for select
		  portion, 
		  PR_debut, 
		  sens
	from #TMP_SEG
	         
	open Segment
	         
	fetch Segment into @vl_Portion, @vl_PR_debut, @vl_Sens
	 
	/*A
	** Tant que le curseur n'est pas vide
	*/
	 
	while (@@sqlstatus = 0)
	begin
	   
		/*A
		** Recherche du PR de fin de segment :
		** PRfin(segment) = Prdebut(segment suivant)
		 */
	
		select @vl_PRf = null
	    
		select @vl_PRf = min(CFG..RES_SEG.PR_debut) from CFG..RES_SEG
		where portion  = @vl_Portion
		and sens     = @vl_Sens
		and PR_debut > @vl_PR_debut
	    
		if @vl_PRf = null
		   select @vl_PRf = CFG..RES_POR.PR_fin from CFG..RES_POR
		   where CFG..RES_POR.numero = @vl_Portion
	         
	    
		/*A
		** Mise a jour du PR de fin trouve dans la table RES_SEG 
		*/
   
		insert #TMP_SEG_RES
		values  (@vl_Portion,@vl_PR_debut,@vl_PRf,@vl_Sens)
	 
		 fetch Segment into @vl_Portion, @vl_PR_debut, @vl_Sens
	 
	end
	 
	/*A
	** Fermeture du curseur sur la table RES_SEG
	*/
	 
	close Segment


	update CFG..RES_SEG
	set PR_fin=#TMP_SEG_RES.PR_fin
	from CFG..RES_SEG, #TMP_SEG_RES
	where #TMP_SEG_RES.portion=CFG..RES_SEG.portion and #TMP_SEG_RES.PR_debut=CFG..RES_SEG.PR_debut and #TMP_SEG_RES.sens=CFG..RES_SEG.sens

	delete #TMP_SEG
		
end	/* fin de traitement dans CFG */

/* travail dans CFT */

else
begin

	/*A
	** Declaration d'un curseur sur la table RES_SEG
	*/

	insert #TMP_SEG
	select  
		CFT..RES_SEG.portion, 
		CFT..RES_SEG.PR_debut, 
		CFT..RES_SEG.sens
	from CFT..RES_SEG
         
	declare Segment cursor 
	for select
		  portion, 
		  PR_debut, 
		  sens
	from #TMP_SEG
	         
	open Segment
	         
	fetch Segment into @vl_Portion, @vl_PR_debut, @vl_Sens
	 
	/*A
	** Tant que le curseur n'est pas vide
	*/
	 
	while (@@sqlstatus = 0)
	begin
	   
		/*A
		** Recherche du PR de fin de segment :
		** PRfin(segment) = Prdebut(segment suivant)
		 */
	
		select @vl_PRf = null
	    
		select @vl_PRf = min(CFT..RES_SEG.PR_debut) from CFT..RES_SEG
		where portion  = @vl_Portion
		and sens     = @vl_Sens
		and PR_debut > @vl_PR_debut
	    
		if @vl_PRf = null
		   select @vl_PRf = CFT..RES_POR.PR_fin from CFT..RES_POR
		   where CFT..RES_POR.numero = @vl_Portion
	         
	    
		/*A
		** Mise a jour du PR de fin trouve dans la table RES_SEG 
		*/
   
		insert #TMP_SEG_RES
		values  (@vl_Portion,@vl_PR_debut,@vl_PRf,@vl_Sens)
	 
		 fetch Segment into @vl_Portion, @vl_PR_debut, @vl_Sens
	 
	end
	 
	/*A
	** Fermeture du curseur sur la table RES_SEG
	*/
	 
	close Segment


	update CFT..RES_SEG
	set PR_fin=#TMP_SEG_RES.PR_fin
	from CFT..RES_SEG, #TMP_SEG_RES
	where #TMP_SEG_RES.portion=CFT..RES_SEG.portion and #TMP_SEG_RES.PR_debut=CFT..RES_SEG.PR_debut and #TMP_SEG_RES.sens=CFT..RES_SEG.sens

	delete #TMP_SEG

end	/* fin de traitement dans CFT */		
	 
return XDC_OK
         
go  
