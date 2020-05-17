data : 讀入資料的型態
	coor : 座標，或者說每筆資料的參數
	group :	每筆資料的所屬群，即行星
	distance : 距離資料和所屬群行星的距離

km : k-means的演算法物件
	filename : 讀入的資料名稱
	quantity : 資料數量(每筆)
	dimension : 每筆資料的維度
	runs : 執行次數
	iteration : 迭代次數
	num_centroid : 行星數量
	map_centroid : 每個行星內所包含的資料
	centroids : 行星，內含每個行星的座標
	inputs : 讀入的資料
	sse : 每筆資料距離所屬群的距離平方

km::function : km內含的函式
	start() : 呼叫km演算法物件執行動作，在外部只有這個函式可以呼叫
	initialize() : 初始化全部的資料，會呼叫read()跟update()
	read() : 讀入資料，並初始化每筆資料的所屬群
	update() : 更新行星的位置
	assign() : 指派每筆資料到新的所屬群
	which_group() : 計算每筆資料到各個行星的距離，選出最小值，並回傳所屬的群號
	show_result() : 展示結果，你可以自己更改內容成看得懂的，最後再做統整

執行參數輸入 : ./執行檔名 km 資料檔名 資料筆數 維度 執行次數 迭代次數 行星數
範例 : ./sample km iris.data.txt 30 4 1 20 3 
