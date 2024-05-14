/*E*/
/*  Fichier : $Id: xzae220sp.prc,v 1.1 2008/11/17 11:23:28 pc2dpdy Exp $      Release : $Revision: 1.1 $      Date : $Date: 2008/11/17 11:23:28 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae220sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* localisation des voies
* Remarque :                                                                   
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   11/10/08        creation DEM/835
-----------------------------------------------------*/
/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE220
* 
* Arguments en entree
*  int numero_fmc    
*  tinyint cle 
* 
* Arguments en sortie
*
* aucun : 
* 
* Service rendu
*
* 
* Code retour
* XDC_OK
* XDC_NOK	:
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

drop procedure XZAE220
go

create procedure XZAE220
        @va_numero_fmc int ,
	@va_cle_fmc    tinyint   ,
        @va_VR             tinyint output      ,
        @va_VM2            tinyint output      , 
        @va_VM1            tinyint output      ,
	@va_VL             tinyint output      ,
	@va_BAU            tinyint output      ,
        @va_VR_I           tinyint output      ,
        @va_VM2_I          tinyint output      ,
	@va_VM1_I          tinyint output      ,
        @va_VL_I           tinyint output      ,
        @va_BAU_I          tinyint output      ,
	@va_Point_Car	   tinyint output,
	@va_Num_Point_Car	int output,
	@va_Nb_Entrees		tinyint output,
	@va_Nb_Sorties		tinyint output
as

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      recuperation des infos en base                                                  */
/************************************************************************************************************************/
/************************************************************************************************************************/

select @va_Nb_Entrees=0
select @va_Nb_Sorties=0

select @va_VR=VR, @va_VM2=VM2,@va_VM1=VM1,@va_VL=VL,@va_BAU=BAU,
       @va_VR_I=VR_I, @va_VM2_I=VM2_I,@va_VM1_I=VM1_I,@va_VL_I=VL_I,@va_BAU_I=BAU_I,
       @va_Point_Car=point_caracteristique, @va_Num_Point_Car=numero_point_caracteristique
       from EXP..FMC_HIS_DER where numero=@va_numero_fmc and cle=@va_cle_fmc

if @va_Point_Car=XDC_POINT_CARACT_ECHANGEUR 
begin
	select @va_Nb_Entrees=voies_entree_sens1+voies_entree_sens2,
		@va_Nb_Sorties=voies_sortie_sens1+voies_sortie_sens2
	from CFG..RES_ECH
	where numero=@va_Num_Point_Car
end
return (XDC_OK)

go
