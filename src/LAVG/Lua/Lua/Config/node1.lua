--[[
	文字剧情节点

	[1] =
	{
		NodeID = 5,				--当前节点ID
		PreNeedNode = {			--前置需要完成的节点ID(每一组可填写多个前置,完成任意一组可解锁该点)	
			[1] = {1,2}
			[2] = {3}
		},						--该样例表示"同时解锁节点1,2"或 "解锁节点3"都可以进入该节点

		Content = {				--文本内容 (会顺序执行)
			{ 	
				roleAId = 1,		--当前说话角色Id (对应role_config)
				roleAState = 1,		--当前说话角色状态
				roleAPos = 0,		--当前说话角色所在位置索引(对应ingame_layout)
				roleBId = 0,		--当前2号角色Id		(若有则填写该项)
				roleBState = 0,	--当前2号角色状态	(若有则填写该项)
				roleBPos = 0,		--当前2号角色所在位置索引
				BgId = 0,			--有则设置背景Id(节点开始必填,之后可选)(对应res_path)
				TextBgId = 0,		--有则设置文本背景Id(节点开始必填,之后可选)(对应res_path)
				Text = "测试文本",
			},
			{ 
				roleAId = 1,
				roleAstate = 1,
				Text = "说的第二句话",
			},
		},

		NextNode = 
		{
			[1] = {"点击此处到节点2",2},
			[2] = {"点击此处到节点3",3},
			default = 4,	--默认节点，若其它几个选项无法跳转节点则跳至该节点(前置节点不满足)
							--若不应该跳至该点时也建议写一个通用的错误节点,方便查错
		}
	}

]]


local nodes = nodes or {}

nodes = {
	[1] =
	{
		NodeID = 1,				--当前节点ID
		PreNeedNode = {},		--前置需要完成的节点ID(每一组可填写多个前置,完成任意一组可解锁该点)		
		Content = {				--文本内容 (会顺序执行)
			{ 	
				RoleAId = 1,		--当前说话角色Id
				RoleAState = 1,	--当前说话角色状态
				RoleAPos = 8,
				RoleBId = 2,		--当前2号角色Id		(若有则填写该项)
				RoleBState = 1,	--当前2号角色状态	(若有则填写该项)
				RoleBPos = 9,
				BgId = 9,			--有则设置背景Id(节点开始必填,之后可选),
				TextNameBgId = 11,	--有则设置人物名称Id(节点开始必填,之后可选),
				TextBgId = 12,		--有则设置文本背景Id(节点开始必填,之后可选),
				Text = "测试文本发生的范德萨范德萨范德萨范德萨跨境购爱的世界工商登记大傻瓜我欧迪斯按加工is大机构is见到过自己的扫过发生大佛是否还是该",
			},
			{ 
				RoleAId = 1,
				RoleAState = 1,
				Text = "说的第二句话",
			},
		},

		NextNode = 
		{
			[1] = {"点击此处到节点2",2},
			[2] = {"点击此处到节点3",3},
			default = 4,	--默认节点，若其它几个选项无法跳转节点则跳至该节点(前置节点不满足)
							--若不应该跳至该点时也建议写一个通用的错误节点,方便查错
		}
	},
	[2] =
	{
		NodeID = 2,				--当前节点ID
		PreNeedNode = {1},		--前置需要完成的节点ID(数组，可填写多个)		
		Content = {				--文本内容
			{ 	
				Player1Id = 1,		--当前说话角色Id
				Player1State = 1,	--当前说话角色状态
				Player2Id = 0,		--当前2号角色Id		(若有则填写该项)
				Player2State = 0,	--当前2号角色状态	(若有则填写该项)
				BGId = 0,			--有则(可选)设置背景Id,
				Text = "节点2",
			},
		},

		NextNode = 
		{
			default = 4,	--默认节点，若其它几个选项无法跳转节点则跳至该节点(前置节点不满足)
		}
	},
}

return nodes
