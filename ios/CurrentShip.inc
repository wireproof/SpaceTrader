static void commanderSkills(UIView *parent,CGFloat x,CGFloat y,CGFloat width,int pilot,int fighter,int trader,int engineer,const int*base){
 NSArray*names=@[@"Pilot",@"Fighter",@"Trader",@"Engineer"];NSArray*symbols=@[@"location.fill",@"bolt.fill",@"arrow.left.arrow.right",@"wrench.fill"];int values[]={pilot,fighter,trader,engineer};CGFloat gap=8,cell=(width-gap)/2;
 for(int i=0;i<4;i++){CGFloat cx=x+(i%2)*(cell+gap),cy=y+(i/2)*48;UIImageView*icon=[[UIImageView alloc]initWithImage:[UIImage systemImageNamed:symbols[i]]];icon.tintColor=UIColor.systemBlueColor;icon.frame=CGRectMake(cx,cy+4,16,16);[parent addSubview:icon];UILabel*label=nativeLabel(parent,names[i],CGRectMake(cx+24,cy,(cell-24)*.52,24),YES);label.font=[UIFont systemFontOfSize:14 weight:UIFontWeightSemibold];label.textAlignment=NSTextAlignmentLeft;label.numberOfLines=1;UILabel*value=nativeLabel(parent,base?[NSString stringWithFormat:@"%d → %d",base[i],values[i]]:[NSString stringWithFormat:@"%d",values[i]],CGRectMake(cx+24+(cell-24)*.52,cy,(cell-24)*.48,24),NO);value.font=[UIFont systemFontOfSize:14];value.textAlignment=NSTextAlignmentRight;value.numberOfLines=1;UIProgressView*bar=[[UIProgressView alloc]initWithProgressViewStyle:UIProgressViewStyleDefault];bar.frame=CGRectMake(cx,cy+30,cell,4);bar.progress=MIN(1,MAX(0,values[i]/10.0));bar.accessibilityLabel=names[i];bar.accessibilityValue=[NSString stringWithFormat:@"%d of 10",values[i]];[parent addSubview:bar];}
}
// Current vessel overview, using the live game state and original navigation.
static void nativeCurrentShip(CGFloat width,CGFloat height){
 CGFloat w=width-24;BOOL wide=width>=700;CGFloat cw=wide?(w-12)*.5:w;
 shipyardCard(nativeSurface,CGRectMake(12,8,cw,674));nativeSurface.subviews.lastObject.tag=43002;
 shipSlots(nativeSurface,CGRectMake(wide?24+cw:12,wide?8:694,cw,220));
 CGFloat y=0;NSMutableArray*items=[NSMutableArray new];
 for(int k=0;k<3;k++){int count=k==0?MAXWEAPON:k==1?MAXSHIELD:MAXGADGET;for(int j=0;j<count;j++){int t=k==0?Ship.Weapon[j]:k==1?Ship.Shield[j]:Ship.Gadget[j];if(t<0)continue;NSString*name=k==0?str(Weapontype[t].Name):k==1?str(Shieldtype[t].Name):str(Gadgettype[t].Name);int icon=k==0?(t<3?t:-1):k==1?(t<2?t+3:-1):(t<5?t+5:-1);[items addObject:@{@"name":name.capitalizedString,@"icon":@(icon)}];}}
 if(EscapePod)[items addObject:@{@"name":@"Escape Pod",@"icon":@(-1)}];
 UIView*slots=[nativeSurface viewWithTag:43001];
 int columns=cw>=500?3:2;CGFloat tileW=(cw-32-(columns-1)*8)/columns;CGFloat loadoutY=40,loadoutHeight=items.count?ceil(items.count/(double)columns)*116:28,slotsHeight=loadoutY+loadoutHeight+68;
 CGRect sf=slots.frame;sf.size.height=slotsHeight;slots.frame=sf;
 UIView*loadout=[UIView new];loadout.frame=CGRectMake(16,loadoutY,cw-32,loadoutHeight);loadout.tag=43003;[slots addSubview:loadout];

 if(!items.count)nativeLabel(loadout,@"No equipment installed",CGRectMake(0,0,cw-32,24),NO);
 for(int i=0;i<items.count;i++){NSDictionary*item=items[i];int icon=[item[@"icon"]intValue];UIView*tile=nativeCard(loadout,CGRectMake((i%columns)*(tileW+8),(i/columns)*116,tileW,108));tile.backgroundColor=UIColor.tertiarySystemGroupedBackgroundColor;UIImageView*im=[[UIImageView alloc]initWithImage:icon>=0?equipmentArt(icon):[UIImage systemImageNamed:@"gearshape"]];im.contentMode=UIViewContentModeScaleAspectFit;im.frame=CGRectMake((tileW-64)/2,8,64,52);[tile addSubview:im];nativeLabel(tile,item[@"name"],CGRectMake(6,64,tileW-12,36),YES).font=[UIFont systemFontOfSize:14 weight:UIFontWeightSemibold];}
 for(UIView*child in slots.subviews)if([child isKindOfClass:UIButton.class])child.frame=CGRectMake(16,sf.size.height-56,cw-32,40);
 y=MAX(CGRectGetMaxY(slots.frame),CGRectGetMaxY([nativeSurface viewWithTag:43002].frame))+12;
 UIView*cmd=dashboardPanel(nativeSurface,@"Commander & Crew",CGRectMake(12,y,w,354));cmd.tag=47000;nativeLabel(cmd,str(NameCommander),CGRectMake(16,42,w-32,32),YES).font=[UIFont systemFontOfSize:24 weight:UIFontWeightSemibold];
 int police=0,rep=0;while(police+1<MAXPOLICERECORD&&PoliceRecordScore>=PoliceRecord[police+1].MinScore)police++;while(rep+1<MAXREPUTATION&&ReputationScore>=Reputation[rep+1].MinScore)rep++;
 NSArray*labels=@[@"Credits",@"Debt",@"Net Worth",@"Day",@"Kills",@"Reputation",@"Police Record",@"Difficulty"];
 NSArray*stats=@[[NSString stringWithFormat:@"%ld",Credits],[NSString stringWithFormat:@"%ld",Debt],[NSString stringWithFormat:@"%ld",CurrentWorth()],[NSString stringWithFormat:@"%d",Days],[NSString stringWithFormat:@"%ld",PoliceKills+TraderKills+PirateKills],str(Reputation[rep].Name),str(PoliceRecord[police].Name),str(DifficultyLevel[Difficulty])];
 CGFloat skillsY=commanderStatRows(cmd,labels,stats,86,w)+8;
 nativeLabel(cmd,@"Skills · commander → ship",CGRectMake(16,skillsY,w-32,24),YES).font=[UIFont preferredFontForTextStyle:UIFontTextStyleSubheadline];
 int baseSkills[]={Mercenary[0].Pilot,Mercenary[0].Fighter,Mercenary[0].Trader,Mercenary[0].Engineer};commanderSkills(cmd,16,skillsY+34,w-32,PilotSkill(&Ship),FighterSkill(&Ship),TraderSkill(&Ship),EngineerSkill(&Ship),baseSkills);
 int hire=-1;for(int i=1;i<MAXCREWMEMBER;i++)if(i!=Ship.Crew[1]&&i!=Ship.Crew[2]&&Mercenary[i].CurSystem==COMMANDER.CurSystem){hire=i;break;}
 NSMutableArray*crew=[NSMutableArray new];for(int i=0;i<MAXCREW;i++)if(Ship.Crew[i]>0)[crew addObject:@{@"id":@(Ship.Crew[i]),@"slot":@(i)}];if(hire>=0)[crew addObject:@{@"id":@(hire),@"slot":@(-1)}];
 UIView*roster=[UIView new];roster.frame=CGRectMake(0,skillsY+142,w,44+crew.count*164+(hire<0?34:0));roster.tag=47001;[cmd addSubview:roster];nativeLabel(roster,@"Crew",CGRectMake(16,8,w-32,28),YES);CGRect cf=cmd.frame;cf.size.height=skillsY+142+roster.frame.size.height+12;cmd.frame=cf;
 for(int i=0;i<crew.count;i++){int member=[crew[i][@"id"]intValue],slot=[crew[i][@"slot"]intValue];CREWMEMBER m=Mercenary[member];CGFloat cy=40+i*164;NSString*name=member==0?str(NameCommander):str(MercenaryName[m.NameIndex]);nativeLabel(roster,name,CGRectMake(16,cy,w-132,26),YES);
 nativeLabel(roster,member==0?@"Commander":[NSString stringWithFormat:@"%@ · %d cr/day",slot<0?@"Available to hire":@"On board",MERCENARYHIREPRICE(member)],CGRectMake(16,cy+28,w-32,22),NO).font=[UIFont systemFontOfSize:14];
 commanderSkills(roster,16,cy+56,w-32,m.Pilot,m.Fighter,m.Trader,m.Engineer,NULL);
 UIView*divider=[UIView new];divider.backgroundColor=UIColor.separatorColor;divider.frame=CGRectMake(16,cy+154,w-32,.5);[roster addSubview:divider];
 if(member!=0){int control=slot<0?PersonnelRosterHire0Button:PersonnelRosterFire0Button+slot-1;UIButton*b=nativeButton(roster,slot<0?@"Hire":@"Dismiss",CGRectMake(w-108,cy,92,44),^{dashboardRoute(MenuCommandPersonnelRoster,PersonnelRosterForm,control);});b.tag=30000+control;}}
 if(hire<0)nativeLabel(roster,@"No crew available to hire here.",CGRectMake(16,44+crew.count*164,w-32,26),NO).textColor=UIColor.secondaryLabelColor;
 y=CGRectGetMaxY(cmd.frame)+12;NSArray*titles=@[@"Quests",@"Special Cargo"];int ids[]={CommanderStatusQuestsButton,CommanderStatusSpecialButton};CGFloat bw=(w-8)/2;
 for(int i=0;i<2;i++){int c=ids[i];UIButton*b=nativeButton(nativeSurface,titles[i],CGRectMake(12+i*(bw+8),y,bw,44),^{dashboardRoute(MenuCommandCommanderStatus,CommanderStatusForm,c);});b.tag=30000+c;}
 nativeSurface.contentSize=CGSizeMake(width,MAX(height,y+56));
}
