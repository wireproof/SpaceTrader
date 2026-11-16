// Current vessel overview, using the live game state and original navigation.
static void nativeCurrentShip(CGFloat width,CGFloat height){
 CGFloat w=width-24;BOOL wide=width>=700;CGFloat cw=wide?(w-12)*.5:w;
 nativeLabel(nativeSurface,@"Dock & commander",CGRectMake(12,8,w,40),YES).font=[UIFont systemFontOfSize:28 weight:UIFontWeightBold];
 shipyardCard(nativeSurface,CGRectMake(12,56,cw,406));nativeSurface.subviews.lastObject.tag=43002;
 shipSlots(nativeSurface,CGRectMake(wide?24+cw:12,wide?56:474,cw,wide?406:252));
 CGFloat y=wide?474:738;NSMutableArray*items=[NSMutableArray new];
 for(int k=0;k<3;k++){int count=k==0?MAXWEAPON:k==1?MAXSHIELD:MAXGADGET;for(int j=0;j<count;j++){int t=k==0?Ship.Weapon[j]:k==1?Ship.Shield[j]:Ship.Gadget[j];if(t<0)continue;NSString*name=k==0?str(Weapontype[t].Name):k==1?str(Shieldtype[t].Name):str(Gadgettype[t].Name);int icon=k==0?(t<3?t:-1):k==1?(t<2?t+3:-1):(t<5?t+5:-1);[items addObject:@{@"name":name,@"icon":@(icon)}];}}
 if(EscapePod)[items addObject:@{@"name":@"Escape pod",@"icon":@(-1)}];
 UIView*loadout=dashboardPanel(nativeSurface,@"Installed equipment",CGRectMake(12,y,w,MAX(88,44+items.count*64)));loadout.tag=43003;
 if(!items.count)nativeLabel(loadout,@"No equipment installed",CGRectMake(16,42,w-32,30),NO);
 for(int i=0;i<items.count;i++){NSDictionary*item=items[i];int icon=[item[@"icon"]intValue];UIImageView*im=[[UIImageView alloc]initWithImage:icon>=0?equipmentArt(icon):[UIImage systemImageNamed:@"gearshape"]];im.contentMode=UIViewContentModeScaleAspectFit;im.frame=CGRectMake(16,42+i*64,64,56);[loadout addSubview:im];nativeLabel(loadout,item[@"name"],CGRectMake(92,44+i*64,w-108,48),YES);}
 y+=loadout.frame.size.height+12;
 UIView*cmd=dashboardPanel(nativeSurface,str(NameCommander),CGRectMake(12,y,w,248));cmd.tag=47000;
 int police=0,rep=0;while(police+1<MAXPOLICERECORD&&PoliceRecordScore>=PoliceRecord[police+1].MinScore)police++;while(rep+1<MAXREPUTATION&&ReputationScore>=Reputation[rep+1].MinScore)rep++;
 NSArray*stats=@[[NSString stringWithFormat:@"Credits  %ld",Credits],[NSString stringWithFormat:@"Debt  %ld",Debt],[NSString stringWithFormat:@"Net worth  %ld",CurrentWorth()],[NSString stringWithFormat:@"Day %d · %ld kills",Days,PoliceKills+TraderKills+PirateKills],str(Reputation[rep].Name),str(PoliceRecord[police].Name)];
 for(int i=0;i<6;i++)nativeLabel(cmd,stats[i],CGRectMake(16+(i%2)*(w-24)/2,38+(i/2)*30,(w-40)/2,28),NO).font=[UIFont systemFontOfSize:15];
 nativeLabel(cmd,[NSString stringWithFormat:@"Pilot %d [%d]    Fighter %d [%d]\nTrader %d [%d]    Engineer %d [%d]",COMMANDER.Pilot,PilotSkill(&Ship),COMMANDER.Fighter,FighterSkill(&Ship),COMMANDER.Trader,TraderSkill(&Ship),COMMANDER.Engineer,EngineerSkill(&Ship)],CGRectMake(16,132,w-32,54),NO).font=[UIFont systemFontOfSize:15];
 nativeLabel(cmd,[NSString stringWithFormat:@"%@ · base [effective] skills",str(DifficultyLevel[Difficulty])],CGRectMake(16,190,w-32,24),NO).textColor=UIColor.secondaryLabelColor;
 y+=260;
 int hire=-1;for(int i=1;i<MAXCREWMEMBER;i++)if(i!=Ship.Crew[1]&&i!=Ship.Crew[2]&&Mercenary[i].CurSystem==COMMANDER.CurSystem){hire=i;break;}
 NSMutableArray*crew=[NSMutableArray new];for(int i=0;i<MAXCREW;i++)if(Ship.Crew[i]>=0)[crew addObject:@{@"id":@(Ship.Crew[i]),@"slot":@(i)}];if(hire>=0)[crew addObject:@{@"id":@(hire),@"slot":@(-1)}];
 UIView*roster=dashboardPanel(nativeSurface,@"Crew roster",CGRectMake(12,y,w,44+crew.count*108+(hire<0?34:0)));roster.tag=47001;
 for(int i=0;i<crew.count;i++){int member=[crew[i][@"id"]intValue],slot=[crew[i][@"slot"]intValue];CREWMEMBER m=Mercenary[member];CGFloat cy=40+i*108;NSString*name=member==0?str(NameCommander):str(MercenaryName[m.NameIndex]);nativeLabel(roster,name,CGRectMake(16,cy,w-132,26),YES);
 nativeLabel(roster,member==0?@"Commander":[NSString stringWithFormat:@"%@ · %d cr/day",slot<0?@"Available to hire":@"On board",MERCENARYHIREPRICE(member)],CGRectMake(16,cy+28,w-32,22),NO).font=[UIFont systemFontOfSize:14];
 nativeLabel(roster,[NSString stringWithFormat:@"Pilot %d · Fighter %d\nTrader %d · Engineer %d",m.Pilot,m.Fighter,m.Trader,m.Engineer],CGRectMake(16,cy+52,w-32,46),NO).font=[UIFont systemFontOfSize:14];
 if(member!=0){int control=slot<0?PersonnelRosterHire0Button:PersonnelRosterFire0Button+slot-1;UIButton*b=nativeButton(roster,slot<0?@"Hire":@"Dismiss",CGRectMake(w-108,cy,92,44),^{dashboardRoute(MenuCommandPersonnelRoster,PersonnelRosterForm,control);});b.tag=30000+control;}}
 if(hire<0)nativeLabel(roster,@"No crew available to hire here.",CGRectMake(16,44+crew.count*108,w-32,26),NO).textColor=UIColor.secondaryLabelColor;
 y+=roster.frame.size.height+12;NSArray*titles=@[@"Quests",@"Special cargo"];int ids[]={CommanderStatusQuestsButton,CommanderStatusSpecialButton};CGFloat bw=(w-8)/2;
 for(int i=0;i<2;i++){int c=ids[i];UIButton*b=nativeButton(nativeSurface,titles[i],CGRectMake(12+i*(bw+8),y,bw,44),^{dashboardRoute(MenuCommandCommanderStatus,CommanderStatusForm,c);});b.tag=30000+c;}
 nativeSurface.contentSize=CGSizeMake(width,MAX(height,y+56));
}
