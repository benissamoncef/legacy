/*E*/
/*  Fichier : $Id: xzae172sp.prc,v 1.3 2016/12/09 16:58:59 gesconf Exp $      Release : $Revision: 1.3 $      Date : $Date: 2016/12/09 16:58:59 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae172sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module NOMADE : Retourne les 10 derniers tdp gdes sections
* Remarque :                                                                   
*
------------------------------------------------------
* HISTORIQUE :
*
* Cluchague     13/01/02  : Creation
* Cluchague     03/09/02  : le tdp calcule peut depasser 999 donc sur 5 digits 1.2
* PNI		26/09/16	: ajot inhibition=0 1.3 dem1187
-----------------------------------------------------*/
/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE172
* 
* Arguments en entree
* 
* Arguments en sortie
*
* aucun : 
* 
* Service rendu
*         retourne les temps de parcours grandes sections                             
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

drop procedure XZAE172
go
create procedure XZAE172
as

declare
@vl_horodate char(19),
@vl_tdp1_1   char(25),
@vl_tdp1_2   char(25),
@vl_tdp2_1   char(25),
@vl_tdp2_2   char(25),
@vl_tdp3_1   char(25),
@vl_tdp3_2   char(25),
@vl_tdp4_1   char(25),
@vl_tdp4_2   char(25),
@vl_tdp_l_z1 smallint,
@vl_tdp_l_z2 smallint,
@vl_tdp_l_z3 smallint,
@vl_tdp_l_z4 smallint,
@vl_tdp_l_z5 smallint,
@vl_tdp_l_z6 smallint,
@vl_tdp_l_z7 smallint,
@vl_tdp_l_z8 smallint,
@vl_tdp_l_z9 smallint,
@vl_tdp_l_z10 smallint,
@vl_tdp_l_z11 smallint,
@vl_tdp_l_z12 smallint,
@vl_tdp_l_z13 smallint,
@vl_tdp_l_z14 smallint,
@vl_tdp_l_z15 smallint,
@vl_tdp_l_z16 smallint,
@vl_tdp_l_z17 smallint,
@vl_tdp_l_z18 smallint,
@vl_tdp_l_z19 smallint,
@vl_horodate_debut datetime,
@vl_horodate_fin datetime, 
@vl_boucle  tinyint,
@vl_temps_partiel1 int,
@vl_temps_partiel2 int,
@vl_temps_partiel3 int,
@vl_temps_total1   int,
@vl_temps_total2   int,
@vl_temps_total3   int,
@vl_temps_total4   int,
@vl_temps_total1_mn int,
@vl_temps_total2_mn int,
@vl_temps_total3_mn int,
@vl_temps_total4_mn int,
@vl_temps_total1_s int,
@vl_temps_total2_s int,
@vl_temps_total3_s int,
@vl_temps_total4_s int,
@vl_tdp1           int,
@vl_tdp1_s         int,
@vl_tdp1_mn        int,
@vl_tdp2           int,
@vl_tdp2_s         int,
@vl_tdp2_mn        int,
@vl_tdp3           int,
@vl_tdp3_s         int,
@vl_tdp3_mn        int,
@vl_tdp4           int,
@vl_tdp4_s         int,
@vl_tdp4_mn        int

   create table #TDP_GDE_SECTION
   (
      horodate       char(19)  null,
      tdp1_1         char(25)  null,
      tdp1_2         char(25)  null,
      tdp2_1         char(25)  null,
      tdp2_2         char(25)  null,
      tdp3_1         char(25)  null,
      tdp3_2         char(25)  null,
      tdp4_1         char(25)  null,
      tdp4_2         char(25)  null
   )


/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      recuperation du temps libre pour chaque zdp                                     */
/************************************************************************************************************************/
/************************************************************************************************************************/
select @vl_tdp_l_z1 = TDP_habituel from CFG..EQT_ZDP where numero=1
select @vl_tdp_l_z2 = TDP_habituel from CFG..EQT_ZDP where numero=2
select @vl_tdp_l_z3 = TDP_habituel from CFG..EQT_ZDP where numero=3
select @vl_tdp_l_z4 = TDP_habituel from CFG..EQT_ZDP where numero=4
select @vl_tdp_l_z5 = TDP_habituel from CFG..EQT_ZDP where numero=5
select @vl_tdp_l_z6 = TDP_habituel from CFG..EQT_ZDP where numero=6
select @vl_tdp_l_z7 = TDP_habituel from CFG..EQT_ZDP where numero=7
select @vl_tdp_l_z8 = TDP_habituel from CFG..EQT_ZDP where numero=8
select @vl_tdp_l_z9 = TDP_habituel from CFG..EQT_ZDP where numero=9
select @vl_tdp_l_z10= TDP_habituel from CFG..EQT_ZDP where numero=10
select @vl_tdp_l_z11= TDP_habituel from CFG..EQT_ZDP where numero=11
select @vl_tdp_l_z12= TDP_habituel from CFG..EQT_ZDP where numero=12
select @vl_tdp_l_z13= TDP_habituel from CFG..EQT_ZDP where numero=13
select @vl_tdp_l_z14= TDP_habituel from CFG..EQT_ZDP where numero=14
select @vl_tdp_l_z15= TDP_habituel from CFG..EQT_ZDP where numero=15
select @vl_tdp_l_z16= TDP_habituel from CFG..EQT_ZDP where numero=16
select @vl_tdp_l_z17= TDP_habituel from CFG..EQT_ZDP where numero=17
select @vl_tdp_l_z18= TDP_habituel from CFG..EQT_ZDP where numero=18
select @vl_tdp_l_z19= TDP_habituel from CFG..EQT_ZDP where numero=19

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      creation de l'en-tete                                                           */
/************************************************************************************************************************/
/************************************************************************************************************************/
select @vl_horodate = "date/heure",
       @vl_tdp1_1   = "La Bocca - Antibes",
       @vl_tdp1_2   = "Antibes - La Bocca",
       @vl_tdp2_1   = "Antibes - Nice Prom",
       @vl_tdp2_2   = "St Laurent - Antibes",
       @vl_tdp3_1   = "Nice Prom - Nice Nord",
       @vl_tdp3_2   = "Nice Nord - St Laurent",
       @vl_tdp4_1   = "Nice Nord - La Turbie",
       @vl_tdp4_2   = "La Turbie - Nice Nord"
insert #TDP_GDE_SECTION values ( @vl_horodate,@vl_tdp1_1, @vl_tdp1_2, @vl_tdp2_1,@vl_tdp2_2, 
				 @vl_tdp3_1,@vl_tdp3_2 , @vl_tdp4_1,@vl_tdp4_2)

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                     creation de la ligne des temps libres                                            */
/************************************************************************************************************************/
/************************************************************************************************************************/
select @vl_horodate = "tdp libre (110KM/H)"
select @vl_tdp1   = (171400-159400)*3600/110000
select @vl_tdp1_mn= @vl_tdp1 / 60
select @vl_tdp1_s = @vl_tdp1 - (@vl_tdp1_mn * 60)
select @vl_tdp1_1 = convert (char(5),@vl_tdp1)+" ("+convert (char(3),@vl_tdp1_mn)+"mn "+convert (char(3),@vl_tdp1_s)+"s)"

select @vl_tdp1   = (171400-159400)*3600/110000
select @vl_tdp1_mn= @vl_tdp1 / 60
select @vl_tdp1_s = @vl_tdp1 - (@vl_tdp1_mn * 60)
select @vl_tdp1_2 = convert (char(5),@vl_tdp1)+" ("+convert (char(3),@vl_tdp1_mn)+"mn "+convert (char(3),@vl_tdp1_s)+"s)"

select @vl_tdp2   = (185800-171400)*3600/110000
select @vl_tdp2_mn= @vl_tdp2 / 60
select @vl_tdp2_s = @vl_tdp2 - (@vl_tdp2_mn * 60)
select @vl_tdp2_1 = convert (char(5),@vl_tdp2)+" ("+convert (char(3),@vl_tdp2_mn)+"mn "+convert (char(3),@vl_tdp2_s)+"s)"

select @vl_tdp2   = (185200-171400)*3600/110000
select @vl_tdp2_mn= @vl_tdp2 / 60
select @vl_tdp2_s = @vl_tdp2 - (@vl_tdp2_mn * 60)
select @vl_tdp2_2 = convert (char(5),@vl_tdp2)+" ("+convert (char(3),@vl_tdp2_mn)+"mn "+convert (char(3),@vl_tdp2_s)+"s)"

select @vl_tdp3   = (197500-185800)*3600/110000
select @vl_tdp3_mn= @vl_tdp3 / 60
select @vl_tdp3_s = @vl_tdp3 - (@vl_tdp3_mn * 60)
select @vl_tdp3_1 = convert (char(5),@vl_tdp3)+" ("+convert (char(3),@vl_tdp3_mn)+"mn "+convert (char(3),@vl_tdp3_s)+"s)"

select @vl_tdp3   = (197500-185200)*3600/110000
select @vl_tdp3_mn= @vl_tdp3 / 60
select @vl_tdp3_s = @vl_tdp3 - (@vl_tdp3_mn * 60)
select @vl_tdp3_2 = convert (char(5),@vl_tdp3)+" ("+convert (char(3),@vl_tdp3_mn)+"mn "+convert (char(3),@vl_tdp3_s)+"s)"

select @vl_tdp4   = (208300-197500)*3600/110000
select @vl_tdp4_mn= @vl_tdp4 / 60
select @vl_tdp4_s = @vl_tdp4 - (@vl_tdp4_mn * 60)
select @vl_tdp4_1 = convert (char(5),@vl_tdp4)+" ("+convert (char(3),@vl_tdp4_mn)+"mn "+convert (char(3),@vl_tdp4_s)+"s)"

select @vl_tdp4   = (208300-197500)*3600/110000
select @vl_tdp4_mn= @vl_tdp4 / 60
select @vl_tdp4_s = @vl_tdp4 - (@vl_tdp4_mn * 60)
select @vl_tdp4_2 = convert (char(5),@vl_tdp4)+" ("+convert (char(3),@vl_tdp4_mn)+"mn "+convert (char(3),@vl_tdp4_s)+"s)"

insert #TDP_GDE_SECTION values ( @vl_horodate,@vl_tdp1_1, @vl_tdp1_2, @vl_tdp2_1,@vl_tdp2_2, 
				 @vl_tdp3_1,@vl_tdp3_2 , @vl_tdp4_1,@vl_tdp4_2)

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      recuperation des infos dynamiques en base                                       */
/************************************************************************************************************************/
/************************************************************************************************************************/
select @vl_boucle = 10
select @vl_horodate_debut = dateadd( minute,-6,getdate())
select @vl_horodate_fin = getdate()
while @vl_boucle > 0
begin
  /********************************************************/
  /* section la bocca - antibes 0,3837*Z1 + Z2 + 0,162*Z3 */
  /********************************************************/
/*  select @vl_temps_partiel1 = sum(T.temps)
  from EXP..ZDP_6MN T, CFG..EQT_ZDP Z,CFG..EQT_GEN G 
  where G.type=39 and G.numero=Z.numero
    and T.numero=G.numero and G.PR > 159400 and Z.PR_fin  < 171400 and G.sens=1
    and horodate between @vl_horodate_debut and @vl_horodate_fin            

  select @vl_temps_partiel2 = T.temps*(Z.PR_fin-159400)/(Z.PR_fin-G.PR)  
  from EXP..ZDP_6MN T, CFG..EQT_ZDP Z,CFG..EQT_GEN G 
  where G.type=39 and G.numero=Z.numero
    and T.numero=G.numero and G.PR < 159400 and Z.PR_fin  > 159400 and G.sens=1
    and horodate between @vl_horodate_debut and @vl_horodate_fin            

  select @vl_temps_partiel3 = T.temps*(171400-G.PR)/(Z.PR_fin-G.PR)  
  from EXP..ZDP_6MN T, CFG..EQT_ZDP Z,CFG..EQT_GEN G 
  where G.type=39 and G.numero=Z.numero
    and T.numero=G.numero and G.PR < 171400 and Z.PR_fin  > 171400 and G.sens=1
    and horodate between @vl_horodate_debut and @vl_horodate_fin            

  select @vl_tdp1_1 = convert (char(25),@vl_temps_partiel1+@vl_temps_partiel2+@vl_temps_partiel3)*/

  select @vl_temps_partiel1 = T.temps from EXP..ZDP_6MN T
		     where T.numero=1 and T.horodate between @vl_horodate_debut and @vl_horodate_fin
  select @vl_temps_partiel2 = T.temps from EXP..ZDP_6MN T
		     where T.numero=2 and T.horodate between @vl_horodate_debut and @vl_horodate_fin
  select @vl_temps_partiel3 = T.temps from EXP..ZDP_6MN T
		     where T.numero=3 and T.horodate between @vl_horodate_debut and @vl_horodate_fin
  if @vl_temps_partiel1  is null or @vl_temps_partiel1 = 0 select @vl_temps_partiel1 =@vl_tdp_l_z1
  if @vl_temps_partiel2  is null or @vl_temps_partiel2 = 0 select @vl_temps_partiel2 =@vl_tdp_l_z2
  if @vl_temps_partiel3  is null or @vl_temps_partiel3 = 0 select @vl_temps_partiel3 =@vl_tdp_l_z3
  select @vl_temps_total1    = convert(int,0.3837*@vl_temps_partiel1+@vl_temps_partiel2+0.162*@vl_temps_partiel3)
  select @vl_temps_total1_mn = @vl_temps_total1 / 60
  select @vl_temps_total1_s  = @vl_temps_total1 - (@vl_temps_total1_mn * 60)
  select @vl_tdp1_1 = convert(char(5),@vl_temps_total1) + " (" + 
		      convert(char(2),@vl_temps_total1_mn) + "mn " +
		      convert(char(2),@vl_temps_total1_s) + "s)"

  /*************************************************/
  /* section antibes - la bocca 80 + Z9 + Z8*0,457 */
  /*************************************************/
/*  select @vl_temps_partiel1 = sum(T.temps)
  from EXP..ZDP_6MN T, CFG..EQT_ZDP Z,CFG..EQT_GEN G 
  where G.type=39 and G.numero=Z.numero
    and T.numero=G.numero and G.PR > 159400 and Z.PR_fin  < 171400 and G.sens=2
    and horodate between @vl_horodate_debut and @vl_horodate_fin            

  select @vl_temps_partiel2 = (161850 - 159400)*3600/110000

  select @vl_temps_partiel3 = T.temps*(171400-G.PR)/(Z.PR_fin-G.PR)  
  from EXP..ZDP_6MN T, CFG..EQT_ZDP Z,CFG..EQT_GEN G 
  where G.type=39 and G.numero=Z.numero
    and T.numero=G.numero and G.PR < 171400 and Z.PR_fin  > 171400 and G.sens=2
    and horodate between @vl_horodate_debut and @vl_horodate_fin    

  select @vl_tdp1_2 = convert (char(25),@vl_temps_partiel1+@vl_temps_partiel2+@vl_temps_partiel3)*/

  select @vl_temps_partiel1 = 80                                       
  select @vl_temps_partiel2 = temps        from EXP..ZDP_6MN T where T.numero=9 and horodate between @vl_horodate_debut and @vl_horodate_fin
  select @vl_temps_partiel3 = temps        from EXP..ZDP_6MN T where T.numero=8 and horodate between @vl_horodate_debut and @vl_horodate_fin
  if @vl_temps_partiel2  is null or @vl_temps_partiel2 = 0 select @vl_temps_partiel2 =@vl_tdp_l_z9
  if @vl_temps_partiel3  is null or @vl_temps_partiel3 = 0 select @vl_temps_partiel3 =@vl_tdp_l_z8
  select @vl_temps_total1    = convert(int,@vl_temps_partiel1+@vl_temps_partiel2+0.457*@vl_temps_partiel3)
  select @vl_temps_total1_mn = @vl_temps_total1 / 60
  select @vl_temps_total1_s  = @vl_temps_total1 - (@vl_temps_total1_mn * 60)
  select @vl_tdp1_2 = convert(char(5),@vl_temps_total1) + " (" + 
		      convert(char(2),@vl_temps_total1_mn) + "mn " +
		      convert(char(2),@vl_temps_total1_s) + "s)"

  /**************************************************/
  /* section antibes - nice prom 0,837*Z3 + Z4 + Z5 */
  /**************************************************/
/*  select @vl_temps_partiel1 = sum(T.temps)
  from EXP..ZDP_6MN T, CFG..EQT_ZDP Z,CFG..EQT_GEN G 
  where G.type=39 and G.numero=Z.numero
    and T.numero=G.numero and G.PR > 171400 and Z.PR_fin  <= 185800 and G.sens=1
    and horodate between @vl_horodate_debut and @vl_horodate_fin            

  select @vl_temps_partiel2 = T.temps*(Z.PR_fin-171400)/(Z.PR_fin-G.PR)  
  from EXP..ZDP_6MN T, CFG..EQT_ZDP Z,CFG..EQT_GEN G 
  where G.type=39 and G.numero=Z.numero
    and T.numero=G.numero and G.PR < 171400 and Z.PR_fin  > 171400 and G.sens=1
    and horodate between @vl_horodate_debut and @vl_horodate_fin            

  select @vl_temps_partiel3 = 0

  select @vl_tdp2_1 = convert (char(25),@vl_temps_partiel1+@vl_temps_partiel2+@vl_temps_partiel3)*/

  select @vl_temps_partiel1 = temps        from EXP..ZDP_6MN T where T.numero=3 and horodate between @vl_horodate_debut and @vl_horodate_fin
  select @vl_temps_partiel2 = temps        from EXP..ZDP_6MN T where T.numero=4 and horodate between @vl_horodate_debut and @vl_horodate_fin
  select @vl_temps_partiel3 = temps        from EXP..ZDP_6MN T where T.numero=6 and horodate between @vl_horodate_debut and @vl_horodate_fin
  if @vl_temps_partiel1  is null or @vl_temps_partiel1 = 0 select @vl_temps_partiel1 =@vl_tdp_l_z3
  if @vl_temps_partiel2  is null or @vl_temps_partiel2 = 0 select @vl_temps_partiel2 =@vl_tdp_l_z4
  if @vl_temps_partiel3  is null or @vl_temps_partiel3 = 0 select @vl_temps_partiel3 =@vl_tdp_l_z5
  select @vl_temps_total2    = convert(int,0.837*@vl_temps_partiel1+@vl_temps_partiel2+0.50867052*@vl_temps_partiel3)
  select @vl_temps_total2_mn = @vl_temps_total2 / 60
  select @vl_temps_total2_s  = @vl_temps_total2 - (@vl_temps_total2_mn * 60)
  select @vl_tdp2_1 = convert(char(5),@vl_temps_total2) + " (" + 
		      convert(char(2),@vl_temps_total2_mn) + "mn " +
		      convert(char(2),@vl_temps_total2_s) + "s)"

  /**********************************************************/
  /* section st laurent - antibes 0,321*Z13 + Z7 + 0,542*Z8 */
  /**********************************************************/
/*  select @vl_temps_partiel1 = sum(T.temps)
  from EXP..ZDP_6MN T, CFG..EQT_ZDP Z,CFG..EQT_GEN G 
  where G.type=39 and G.numero=Z.numero
    and T.numero=G.numero and G.PR > 171400 and Z.PR_fin  < 185200 and G.sens=2
    and horodate between @vl_horodate_debut and @vl_horodate_fin            

  select @vl_temps_partiel2 = T.temps*(Z.PR_fin-171400)/(Z.PR_fin-G.PR)  
  from EXP..ZDP_6MN T, CFG..EQT_ZDP Z,CFG..EQT_GEN G 
  where G.type=39 and G.numero=Z.numero
    and T.numero=G.numero and G.PR < 171400 and Z.PR_fin  > 171400 and G.sens=2
    and horodate between @vl_horodate_debut and @vl_horodate_fin            

  select @vl_temps_partiel3 = T.temps*(185200-G.PR)/(Z.PR_fin-G.PR)  
  from EXP..ZDP_6MN T, CFG..EQT_ZDP Z,CFG..EQT_GEN G 
  where G.type=39 and G.numero=Z.numero
    and T.numero=G.numero and G.PR < 185200 and Z.PR_fin  > 185200 and G.sens=2
    and horodate between @vl_horodate_debut and @vl_horodate_fin            

  select @vl_tdp2_2 = convert (char(25),@vl_temps_partiel1+@vl_temps_partiel2+@vl_temps_partiel3)*/

  select @vl_temps_partiel1 = temps        from EXP..ZDP_6MN T where T.numero=13 and horodate between @vl_horodate_debut and @vl_horodate_fin
  select @vl_temps_partiel2 = temps        from EXP..ZDP_6MN T where T.numero=7 and horodate between @vl_horodate_debut and @vl_horodate_fin
  select @vl_temps_partiel3 = temps        from EXP..ZDP_6MN T where T.numero=8 and horodate between @vl_horodate_debut and @vl_horodate_fin
  if @vl_temps_partiel1  is null or @vl_temps_partiel1 = 0 select @vl_temps_partiel1 =@vl_tdp_l_z13
  if @vl_temps_partiel2  is null or @vl_temps_partiel2 = 0 select @vl_temps_partiel2 =@vl_tdp_l_z7
  if @vl_temps_partiel3  is null or @vl_temps_partiel3 = 0 select @vl_temps_partiel3 =@vl_tdp_l_z8
  select @vl_temps_total2    = convert(int,0.321*@vl_temps_partiel1+@vl_temps_partiel2+0.542*@vl_temps_partiel3)
  select @vl_temps_total2_mn = @vl_temps_total2 / 60
  select @vl_temps_total2_s  = @vl_temps_total2 - (@vl_temps_total2_mn * 60)
  select @vl_tdp2_2 = convert(char(5),@vl_temps_total2) + " (" + 
		      convert(char(2),@vl_temps_total2_mn) + "mn " +
		      convert(char(2),@vl_temps_total2_s) + "s)"

  /************************************************************/
  /* section nice prom - nice nord 0,5256*Z6 + Z12 + 0,33*Z19 */
  /************************************************************/
/*  select @vl_temps_partiel1 = sum(T.temps)
  from EXP..ZDP_6MN T, CFG..EQT_ZDP Z,CFG..EQT_GEN G 
  where G.type=39 and G.numero=Z.numero
    and T.numero=G.numero and G.PR > 185800 and Z.PR_fin  < 197500 and G.sens=1
    and horodate between @vl_horodate_debut and @vl_horodate_fin            

  select @vl_temps_partiel2 = T.temps*(Z.PR_fin-185800)/(Z.PR_fin-G.PR)  
  from EXP..ZDP_6MN T, CFG..EQT_ZDP Z,CFG..EQT_GEN G 
  where G.type=39 and G.numero=Z.numero
    and T.numero=G.numero and G.PR < 185800 and Z.PR_fin  > 185800 and G.sens=1
    and horodate between @vl_horodate_debut and @vl_horodate_fin            

  select @vl_temps_partiel3 = T.temps*(197500-G.PR)/(Z.PR_fin-G.PR)  
  from EXP..ZDP_6MN T, CFG..EQT_ZDP Z,CFG..EQT_GEN G 
  where G.type=39 and G.numero=Z.numero
    and T.numero=G.numero and G.PR < 197500 and Z.PR_fin  > 197500 and G.sens=1
    and horodate between @vl_horodate_debut and @vl_horodate_fin            

  select @vl_tdp3_1 = convert (char(25),@vl_temps_partiel1+@vl_temps_partiel2+@vl_temps_partiel3)*/

  select @vl_temps_partiel1 = temps        from EXP..ZDP_6MN T where T.numero=6 and horodate between @vl_horodate_debut and @vl_horodate_fin
  select @vl_temps_partiel2 = temps        from EXP..ZDP_6MN T where T.numero=12 and horodate between @vl_horodate_debut and @vl_horodate_fin
  select @vl_temps_partiel3 = temps        from EXP..ZDP_6MN T where T.numero=19 and horodate between @vl_horodate_debut and @vl_horodate_fin
  if @vl_temps_partiel1  is null or @vl_temps_partiel1 = 0 select @vl_temps_partiel1 =@vl_tdp_l_z6
  if @vl_temps_partiel2  is null or @vl_temps_partiel2 = 0 select @vl_temps_partiel2 =@vl_tdp_l_z12
  if @vl_temps_partiel3  is null or @vl_temps_partiel3 = 0 select @vl_temps_partiel3 =@vl_tdp_l_z19
  select @vl_temps_total3    = convert(int,0.5256*@vl_temps_partiel1+@vl_temps_partiel2+0.33*@vl_temps_partiel3)
  select @vl_temps_total3_mn = @vl_temps_total3 / 60
  select @vl_temps_total3_s  = @vl_temps_total3 - (@vl_temps_total3_mn * 60)
  select @vl_tdp3_1 = convert(char(5),@vl_temps_total3) + " (" + 
		      convert(char(2),@vl_temps_total3_mn) + "mn " +
		      convert(char(2),@vl_temps_total3_s) + "s)"

  /********************************************************/
  /* section nice nord - st laurent 0,877*Z18 + 0,678*Z13 */
  /********************************************************/
/*  select @vl_temps_partiel1 = 0

  select @vl_temps_partiel2 = T.temps*(Z.PR_fin-185200)/(Z.PR_fin-G.PR)  
  from EXP..ZDP_6MN T, CFG..EQT_ZDP Z,CFG..EQT_GEN G 
  where G.type=39 and G.numero=Z.numero
    and T.numero=G.numero and G.PR < 185200 and Z.PR_fin  > 185200 and G.sens=2
    and horodate between @vl_horodate_debut and @vl_horodate_fin            

  select @vl_temps_partiel3 = T.temps*(197500-G.PR)/(Z.PR_fin-G.PR)  
  from EXP..ZDP_6MN T, CFG..EQT_ZDP Z,CFG..EQT_GEN G 
  where G.type=39 and G.numero=Z.numero
    and T.numero=G.numero and G.PR < 197500 and Z.PR_fin  > 197500 and G.sens=2
    and horodate between @vl_horodate_debut and @vl_horodate_fin            

  select @vl_tdp3_2 = convert (char(25),@vl_temps_partiel1+@vl_temps_partiel2+@vl_temps_partiel3)*/

  select @vl_temps_partiel1 = temps        from EXP..ZDP_6MN T where T.numero=18 and horodate between @vl_horodate_debut and @vl_horodate_fin
  select @vl_temps_partiel2 = 0
  select @vl_temps_partiel3 = temps        from EXP..ZDP_6MN T where T.numero=13 and horodate between @vl_horodate_debut and @vl_horodate_fin
  if @vl_temps_partiel1  is null or @vl_temps_partiel1 = 0 select @vl_temps_partiel1 =@vl_tdp_l_z18
  if @vl_temps_partiel3  is null or @vl_temps_partiel3 = 0 select @vl_temps_partiel3 =@vl_tdp_l_z13
  select @vl_temps_total3    = convert(int,0.877*@vl_temps_partiel1+@vl_temps_partiel2+0.678*@vl_temps_partiel3)
  select @vl_temps_total3_mn = @vl_temps_total3 / 60
  select @vl_temps_total3_s  = @vl_temps_total3 - (@vl_temps_total3_mn * 60)
  select @vl_tdp3_2 = convert(char(5),@vl_temps_total3) + " (" + 
		      convert(char(2),@vl_temps_total3_mn) + "mn " +
		      convert(char(2),@vl_temps_total3_s) + "s)"

  /*********************************************/
  /* section nice nord - turbie 134 + 0,67*Z19 */
  /*********************************************/
/*  select @vl_temps_partiel1 = 0              

  select @vl_temps_partiel2 = T.temps*(Z.PR_fin-197500)/(Z.PR_fin-G.PR)  
  from EXP..ZDP_6MN T, CFG..EQT_ZDP Z,CFG..EQT_GEN G 
  where G.type=39 and G.numero=Z.numero
    and T.numero=G.numero and G.PR < 197500 and Z.PR_fin  > 197500 and G.sens=1
    and horodate between @vl_horodate_debut and @vl_horodate_fin            

  select @vl_temps_partiel3 = (208300-204190)*3600/110000                    

  select @vl_tdp4_1 = convert (char(25),@vl_temps_partiel1+@vl_temps_partiel2+@vl_temps_partiel3)*/

  select @vl_temps_partiel1 = 0
  select @vl_temps_partiel2 = 134
  select @vl_temps_partiel3 = temps        from EXP..ZDP_6MN T where T.numero=19 and horodate between @vl_horodate_debut and @vl_horodate_fin
  if @vl_temps_partiel3  is null or @vl_temps_partiel3 = 0 select @vl_temps_partiel3 =@vl_tdp_l_z19
  select @vl_temps_total4    = convert(int,@vl_temps_partiel1+@vl_temps_partiel2+0.67*@vl_temps_partiel3)
  select @vl_temps_total4_mn = @vl_temps_total4 / 60
  select @vl_temps_total4_s  = @vl_temps_total4 - (@vl_temps_total4_mn * 60)
  select @vl_tdp4_1 = convert(char(5),@vl_temps_total4) + " (" + 
		      convert(char(2),@vl_temps_total4_mn) + "mn " +
		      convert(char(2),@vl_temps_total4_s) + "s)"

  /***************************************************/
  /* section turbie - nice nord Z17 + 42 + 0,122*Z18 */
  /***************************************************/
/*  select @vl_temps_partiel1 = sum(T.temps)
  from EXP..ZDP_6MN T, CFG..EQT_ZDP Z,CFG..EQT_GEN G 
  where G.type=39 and G.numero=Z.numero
    and T.numero=G.numero and G.PR > 197500 and Z.PR_fin  < 208300 and G.sens=2
    and horodate between @vl_horodate_debut and @vl_horodate_fin            

  select @vl_temps_partiel2 = T.temps*(Z.PR_fin-197500)/(Z.PR_fin-G.PR)  
  from EXP..ZDP_6MN T, CFG..EQT_ZDP Z,CFG..EQT_GEN G 
  where G.type=39 and G.numero=Z.numero
    and T.numero=G.numero and G.PR < 197500 and Z.PR_fin  > 197500 and G.sens=2
    and horodate between @vl_horodate_debut and @vl_horodate_fin            

  select @vl_temps_partiel3 = (208300-207000)*3600/110000                    

  select @vl_tdp4_2 = convert (char(25),@vl_temps_partiel1+@vl_temps_partiel2+@vl_temps_partiel3)*/

  select @vl_temps_partiel1 = temps        from EXP..ZDP_6MN T where T.numero=17 and horodate between @vl_horodate_debut and @vl_horodate_fin
  select @vl_temps_partiel2 = 42
  select @vl_temps_partiel3 = temps        from EXP..ZDP_6MN T where T.numero=18 and horodate between @vl_horodate_debut and @vl_horodate_fin
  if @vl_temps_partiel1  is null or @vl_temps_partiel1 = 0 select @vl_temps_partiel1 =@vl_tdp_l_z17
  if @vl_temps_partiel3  is null or @vl_temps_partiel3 = 0 select @vl_temps_partiel3 =@vl_tdp_l_z18
  select @vl_temps_total4    = convert(int,@vl_temps_partiel1+@vl_temps_partiel2+0.122*@vl_temps_partiel3)
  select @vl_temps_total4_mn = @vl_temps_total4 / 60
  select @vl_temps_total4_s  = @vl_temps_total4 - (@vl_temps_total4_mn * 60)
  select @vl_tdp4_2 = convert(char(5),@vl_temps_total4) + " (" + 
		      convert(char(2),@vl_temps_total4_mn) + "mn " +
		      convert(char(2),@vl_temps_total4_s) + "s)"

  select @vl_horodate = convert (char(10),@vl_horodate_fin,103) + ' ' + convert(char(5),@vl_horodate_fin,8)
  insert #TDP_GDE_SECTION values ( @vl_horodate,@vl_tdp1_1, @vl_tdp1_2, @vl_tdp2_1,@vl_tdp2_2, @vl_tdp3_1,@vl_tdp3_2 , @vl_tdp4_1,@vl_tdp4_2)

  select @vl_boucle = @vl_boucle - 1
  select @vl_horodate_debut = dateadd( minute,-6,@vl_horodate_debut)
  select @vl_horodate_fin =   dateadd( minute,-6,@vl_horodate_fin)

end

/*valeurs a retourner*/
select  * from #TDP_GDE_SECTION 

drop table #TDP_GDE_SECTION 

return (XDC_OK)
go
