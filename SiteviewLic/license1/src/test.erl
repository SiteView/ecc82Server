-module(test).



-compile(export_all).

start() -> test().  
stop() -> ok.
test1()->
    license:start(),   
    %~ Pdn = "ECC",
    %~ Machinecode =  "1e3bc4c71d544c34ecc6ab42e88d7a404ae02ee6ad7c7612420c4b8d8a14b4bd6c998d0521c142ae3d9439ac7ed16698ee71aa12b9fe14e9de1dd79589dd1a91353f55b4fc0198426a6d24fa735a59d89aeb376273c8e2ccf9a53a353ecf0202185f9baf63164177de35c23b3e2e8cc0a474a209fe4024e0fcfd624a9c3e588f922d68b165936d837b799c470b456ec3d276ffc9c21cf0b20c004e9cb22f8b6ba435098c6b2a4bcd237fd38072d1eb1e875a2074eb874b6c",
    %~ Customer = [229,134,133,233,131,168,229,145,152,229,183,165],
    %~ Modules = [230,149,180,228,189,147,232,167,134,229,155,190,44,
                           %~ 230,160,145,231,138,182,232,167,134,229,155,190,44,
                           %~ 230,139,147,230,137,145,232,167,134,229,155,190,44,
                           %~ 230,139,147,230,137,145,230,137,171,230,143,143,44,
                           %~ 229,183,165,229,141,149,44,230,138,165,232,161,168,
                           %~ 44,229,145,138,232,173,166,44,232,174,190,231,189,
                           %~ 174],
    %~ Points = "200",
    %~ Devices = "50",
    %~ Startdate = "2010-8-19",
    %~ Delaydays = "30",
    %~ User = "zhenming.wang",
    %~ Support = "bin.liu",
    %~ Isfinal = "1",
    %~ Allowmachine = "1",   
    %~ Isdefault = "1",
    %~ io:format("Machinecode:~p~n~n~n",[license:isvalidmachinecode(Machinecode)]),
    %~ io:format("CDKEY:~p~n~n~n",[license:createkey(Machinecode)]),
    %~ io:format("License:~p~n",[license:createlicense(Pdn,Machinecode,Customer,Modules,Points,Devices,Startdate,Delaydays,User,Support,Isfinal,Allowmachine,Isdefault)]),
    
    
    
    {ok,License} = license:getlicstring("SiteViewECC.lic"),
    %%{ok,License} = license:getlicstring("SiteViewITSM.Lic"),
    io:format("License:~p~n~n~n",[License]),
    io:format("decrypt2data:~p~n~n~n",[license:decrypt2data(License)]),
    io:format("isvalidlicense:~p~n~n~n",[license:isvalidlicense("",License,0,"true")]),
    ok.

test()->
    license:start(),   
    Pdn = "ECC",
    Machinecode =  "1e3bc4c71d544c34ecc6ab42e88d7a404ae02ee6ad7c7612420c4b8d8a14b4bd6c998d0521c142ae3d9439ac7ed16698ee71aa12b9fe14e9de1dd79589dd1a91353f55b4fc0198426a6d24fa735a59d89aeb376273c8e2ccf9a53a353ecf0202185f9baf63164177de35c23b3e2e8cc0a474a209fe4024e0fcfd624a9c3e588f922d68b165936d837b799c470b456ec3d276ffc9c21cf0b20c004e9cb22f8b6ba435098c6b2a4bcd237fd38072d1eb1e875a2074eb874b6c",
	%Machinecode =  "ac8f8f7133e4e95d873c362e9f5e4a61ddbe17924fdfba65725dea23828ce6714cc37057b2a932839ae35024601b9b73ca21cfb27ff97e2c88b38d4f973f06b637450095d29370a7b3904392761c65d58813ca8f9bc40cb9b1efcc24a59f50c106c7db090e9776846fcde3fb0e11cf24276044acdf91d1dea4b92f9b071de107484609424cb1ab6ee1e3d5f60565625f9ee727c149514e583c12b30ece2d91b42594654452e2adb1daca61b2b88a0568f380392039a31468d10ee291a128f80472e6df08184a314e1699130465a3ee163c24831d28ffc336fa5948e7a182a9de1ab8053c13a2c37fa857e427d033f7d9c351c1201008eb53c13b8932b443e2e5df8f6fdb1128bac37cf5f1b808b48e208203a57e529ffa3b",
    Customer = [229,134,133,233,131,168,229,145,152,229,183,165],
    Modules = [230,149,180,228,189,147,232,167,134,229,155,190,44,
                            230,160,145,231,138,182,232,167,134,229,155,190,44,
                            230,139,147,230,137,145,232,167,134,229,155,190,44,
                            230,139,147,230,137,145,230,137,171,230,143,143,44,
                            229,183,165,229,141,149,44,230,138,165,232,161,168,
                            44,229,145,138,232,173,166,44,232,174,190,231,189,
                            174],
     Points = "200",
     Devices = "50",
     Startdate = "2010-8-19",
     Delaydays = "30",
     User = "zhenming.wang",
     Support = "bin.liu",
     Isfinal = "1",
     Allowmachine = "1",   
     Isdefault = "1",
     io:format("Machinecode:~p~n~n~n",[license:isvalidmachinecode(Machinecode)]),
     io:format("CDKEY:~p~n~n~n",[license:createkey(Machinecode)]),
     io:format("License:~p~n",[license:createlicense(Pdn,Machinecode,Customer,Modules,Points,Devices,Startdate,Delaydays,User,Support,Isfinal,Allowmachine,Isdefault)]),
    
     ok.

		