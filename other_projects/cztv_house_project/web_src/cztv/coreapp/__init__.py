# -*- coding: utf-8 -*-
        
def init_field(category, name, label, field_type, values='',
               visiable_order=0):
    from cztv.coreapp.models import FieldTemplate 
    f, c = FieldTemplate.objects.get_or_create(name=name)
    if c:
        f.category = category
        f.label = label
        f.field_type = field_type
        f.values = values
        f.visiable_order = visiable_order
        f.save()

def initialize():
    # Create your models here.
    pass
    f = init_field
    
    #物业属性
    f("wysx", "fwxz", "房屋性质", "choice", "01,新房;02,二手房")
    f("wysx", "sfzs", "是否在售", "multi_choice", "01,在售;02,待售;03,预售;04,售罄;05,在租;")
    f("wysx", "wylb", "物业类别", "choice", "01,普通住宅;02,别墅;03,公寓;04,经济适用房;05,商住楼;06,其他")
    f("wysx", "jzlb", "建筑类别", "choice", "01,低层;02,多层;03,小高层;04,高层;05,超高层")
    f("wysx", "fwxz", "房屋现状", "choice", "01,期房;02,现房")
    f("wysx", "zxqk", "装修情况", "choice", "01,全装修;02,精装修;03,厨卫装修;04,毛坯")
    f("wysx", "kfzq", "开发周期", "text", "")
    f("wysx", "xsl", "销售率", "text", "")
    
    #地理位置
    f("dlwz", "cqwz", "城区位置", "choice", "01,城东;02,城南;03,城西;04,城北;05,城中;06,下属五县市;")
    #交通状况
    f("jtzk", "jtyd", "所处交通要道", "text")
    
    #项目特色
    f("xmts", "xmts", "", "multi_choice", """01,创意地产;02,水景地产;03,复合地产 ;04,特色别墅 ;
            05,豪华居住区;06,宜居生态地产;07,国际化社区;08,公园地产 ;09,旅游地产;10,地铁沿线;11,低密居所;
            12,配套商品房;13,动迁房;14,花园洋房;15,科技住宅;16,景观居所;17,经济住宅;18,酒店式公寓;
            19,回迁房;20,小户型 ;21,投资地产;22,中式地产 ;23,成品地产 ;24,教育地产;25,海景地产;
            """)
    
    #工程进度
    f("gcjd", "kgsj", "开工时间", "text", "")
    f("gcjd", "jgsj", "竣工时间", "text", "")
    f("gcjd", "kpsj", "开盘时间", "text", "")
    f("gcjd", "rzsj", "入住时间", "text", "")
    
    #物业规模
    f("wygm", "zdmz", "占地面积", "text", "")
    f("wygm", "zjmz", "建筑面积", "text", "")
    f("wygm", "rjl", "容积率", "text", "")
    f("wygm", "lhl", "绿化率", "text", "")
    f("wygm", "lczk", "楼层状况", "text", "")
    f("wygm", "zfs", "总户数", "text", "")
        
    #最新价格
    f("zxjg", "qj", "起价", "text", "")
    f("zxjg", "zgj", "最高价", "text", "")
    f("zxjg", "jj", "均价", "text", "")
    f("zxjg", "zj", "总结", "text", "")
    f("zxjg", "ajgd", "付款方式/按揭规定", "text", "")     
    
    #物业服务
    f("wyfw", "cwsl", "车位数量", "text", "")
    f("wyfw", "csjg", "出售价格", "text", "")
    f("wyfw", "cwb", "车位比", "text", "")
    f("wyfw", "czjg", "出租价格", "text", "")
    f("wyfw", "wyglf", "物业管理费", "text", "")  
    f("wyfw", "wygs", "物业公司", "text", "")     
    
    #周边配套
    f("zbpt", "zbxx", "周边学校", "text", "")
    f("zbpt", "zhsc", "综合商场", "text", "")
    f("zbpt", "yh", "银行", "text", "")
    f("zbpt", "yj", "邮局", "text", "")
    f("zbpt", "yy", "医院", "text", "")  
    f("zbpt", "qt", "其他", "text", "")

    #建材设备
    f("jcsb", "jcjg", "结构", "text", "")
    f("jcsb", "jcgs", "供水", "text", "")
    f("jcsb", "jcwq", "外墙", "text", "")
    f("jcsb", "jcgd", "供电", "text", "")
    f("jcsb", "jcnq", "内墙", "text", "")  
    f("jcsb", "jcgq", "供气", "text", "")
    f("jcsb", "jcdt", "电梯", "text", "")
    f("jcsb", "jccn", "采暖", "text", "")
    f("jcsb", "jcdtfs", "电梯户数", "text", "")
    f("jcsb", "jctx", "通讯", "text", "")
    f("jcsb", "jccf", "门窗", "text", "")
    f("jcsb", "jcwsf", "卫生间", "text", "")
    f("jcsb", "jcdt", "大堂", "text", "")
    f("jcsb", "jcchuf", "厨房", "text", "")
    f("jcsb", "jcabxt", "安保系统", "text", "")
    f("jcsb", "jcznss", "智能化设施", "text", "")   
    f("jcsb", "jcxfxt", "空调新风系统", "text", "")    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
        