use PRC
go

if exists (select * from sysobjects where name = "XZAO1490" and type = "P")
	drop procedure XZAO1490
go


create procedure XZAO1490
   @va_Basedd_in       	char(3),
   @va_NumeroPortion_in smallint,
   @va_NomTroncon_in	char(25)
   as

/* 
*
* SP	XZAO1490SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in			
* XDY_Mot	va_NumeroPortion_in
* XDY_Nom	va_NomTroncon_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :RN
*
*  Liste des Echangeurs d'un Troncon ou d'une portion ou de l'ensemble des autoroutes
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Si NomTroncon est non values alors la liste des Echangeurs du troncon
*
* Si NumeroPortion est value alors la liste des Echangeurs de la portion
*
* FONCTION :
*
* Retourne XZAOT_Echangeur
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL		30/11/11	: Creation
* PNI 	11/09/23	: Prise en compte du flag RES_ECH.supprime=XDC_FAUX SAE-517
------------------------------------------------------*/
declare

   /*A
   ** Declaration des variables locales
   */

   @vl_Autoroute	tinyint,
   @vl_PRdebut		int,
   @vl_PRfin		int

   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */

   if rtrim ( @va_Basedd_in ) = null 		select @va_Basedd_in = null
   if rtrim ( @va_NomTroncon_in ) = null 	select @va_NomTroncon_in = null
   if @va_NumeroPortion_in = 0		 	select @va_NumeroPortion_in = null



   if @va_Basedd_in = null
   or  (@va_NomTroncon_in = null
    and @va_NumeroPortion_in = null)

      return XDC_ARG_INV 

   else

   begin

    /*A
    **  Travail dans la base CFG
    */

    if @va_Basedd_in = "CFG" 
    begin

      /*A
      ** NomTroncon est nul 
      ** Recherche des Echangeurs de la Portion
      */

      if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null

      select distinct ECH.nom,
                      ECH.numero,
                      ECH.sortie,
                      POR.autoroute,
                      ECH.PR,
                      ECH.peage_entree_sens1,
                      ECH.PR_entree_sens1,
                      ECH.peage_entree_sens2,
                      ECH.PR_entree_sens2,
                      ECH.peage_sortie_sens1,
                      ECH.PR_sortie_sens1,
                      ECH.peage_sortie_sens2,
                      ECH.PR_sortie_sens2

      from CFG..RES_ECH ECH, CFG..RES_POR POR

      where POR.numero = @va_NumeroPortion_in
      and ECH.portion = POR.numero 
      and ECH.supprime=XDC_FAUX
      order by POR.autoroute, ECH.PR

      /*A
      ** NumeroPortion est nul 
      ** Recherche des Echangeurs du Troncon
      */

      if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
      begin

        select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_TRC
        where nom = @va_NomTroncon_in

        select distinct ECH.nom,
                        ECH.numero,
                        ECH.sortie,
                        POR.autoroute,
                        ECH.PR,
                        ECH.peage_entree_sens1,
                        ECH.PR_entree_sens1,
                        ECH.peage_entree_sens2,
                        ECH.PR_entree_sens2,
                        ECH.peage_sortie_sens1,
                        ECH.PR_sortie_sens1,
                        ECH.peage_sortie_sens2,
                        ECH.PR_sortie_sens2
        from CFG..RES_ECH ECH, CFG..RES_POR POR


        where POR.autoroute = @vl_Autoroute
          and POR.PR_debut  <=  @vl_PRfin
          and POR.PR_fin    >= @vl_PRdebut
          and ECH.portion   = POR.numero
          and ECH.PR	  >= @vl_PRdebut
          and ECH.PR	  <= @vl_PRfin
          and ECH.supprime = XDC_FAUX
        order by POR.autoroute, ECH.PR

      end
    end
#ifdef CI
    else

      /*A
      **  Travail dans la base CFT
      */

      begin

      /*A
      ** NomTroncon est nul 
      ** Recherche des Echangeurs de la Portion
      */

      if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null

      select distinct ECH.nom,
                      ECH.numero,
                      ECH.sortie,
                      POR.autoroute,
                      ECH.PR,
                      ECH.peage_entree_sens1,
                      ECH.PR_entree_sens1,
                      ECH.peage_entree_sens2,
                      ECH.PR_entree_sens2,
                      ECH.peage_sortie_sens1,
                      ECH.PR_sortie_sens1,
                      ECH.peage_sortie_sens2,
                      ECH.PR_sortie_sens2
      from CFT..RES_ECH ECH, CFT..RES_POR POR

      where POR.numero = @va_NumeroPortion_in
      and ECH.portion = POR.numero
      and ECH.supprime = XDC_FAUX
      order by POR.autoroute, ECH.PR


      /*A
      ** NumeroPortion est nul 
      ** Recherche des Echangeurs du Troncon
      */

      if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
      begin

        select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_TRC
        where nom = @va_NomTroncon_in

        select distinct ECH.nom,
                        ECH.numero,
                        ECH.sortie,
                        POR.autoroute,
                        ECH.PR,
                        ECH.peage_entree_sens1,
                        ECH.PR_entree_sens1,
                        ECH.peage_entree_sens2,
                        ECH.PR_entree_sens2,
                        ECH.peage_sortie_sens1,
                        ECH.PR_sortie_sens1,
                        ECH.peage_sortie_sens2,
                        ECH.PR_sortie_sens2
        from CFT..RES_ECH ECH, CFT..RES_POR POR

        where POR.autoroute = @vl_Autoroute
            and POR.PR_debut  <=  @vl_PRfin
            and POR.PR_fin    >= @vl_PRdebut
            and ECH.portion   = POR.numero
            and ECH.PR	  >= @vl_PRdebut
            and ECH.PR	  <= @vl_PRfin
            and ECH.supprime = XDC_FAUX
        order by POR.autoroute, ECH.PR

      end
    end

#endif
   end

   return XDC_OK
go
