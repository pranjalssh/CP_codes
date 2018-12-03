static const int dy[4] = {0,1,0,-1}, dx[4] = {1,0,-1,0};
rep(d, 4) {
	int yy = i + dy[d], xx = j + dx[d];
	if(yy<0||yy>=h||xx<0||xx>=w) continue;
	if(board[yy][xx] == '#') continue;
	
}

//方角
const char *dirs = "ESWN";
int d = strchr(dirs, dir) - dirs;
