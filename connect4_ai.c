#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

#include "connect4_ai.h"

#define MAX_DEPTH 18

static int next_move_score_min(char *board, int height, int width, char myself, char opponent, int depth);

static int next_move_score_max(char *board, int height, int width, char myself, char opponent, int depth);

extern int check(char *board, int height, int width, int column)
{
	if (column < 0 || column >= width)
		return -1;

	for (int row = 0; row < height; row++) {
		if (board[row * width + column] != 0)
			return row - 1;
	}
	return height - 1;
}

extern int drop(char *board, int height, int width, int column, char symbol)
{
	int row = check(board, height, width, column);
	if (row < 0)
		return -1;

//	printf("drop flag %d\n", row);

	board[row * width + column] = symbol;
	return row;	
}

static void pickup(char *board, int width, int column, int row)
{
	assert(board[row * width + column] != 0);

	board[row * width + column] = 0;
}

extern bool win(char *board, int height, int width, int row, int column)
{
	char symbol = board[row * width + column];

	int count = 0;
	for (int r = row; r < height; r++) {
		if (board[r * width + column] != symbol)
			break;
		count++;
	}
	for (int r = row; r >= 0; r--) {
		if (board[r * width + column] != symbol)
			break;
		count++;
	}
	if (count > 4)
		return true;

	count = 0;
	for (int c = column; c < width; c++) {
		if (board[row * width + c] != symbol)
			break;
		count++;
	}
	for (int c = column; c >= 0; c--) {
		if (board[row * width + c] != symbol)
			break;
		count++;
	}
	if (count > 4)
		return true;

	count = 0;
	for (int r = row, c = column; r < height && c < width; r++, c++) {
		if (board[r * width + c] != symbol)
			break;
		count++;
	}
	for (int r = row, c = column; r >= 0 && c >= 0; r--, c--) {
		if (board[r * width + c] != symbol)
			break;
		count++;
	}
	if (count > 4)
		return true;

	count = 0;
	for (int r = row, c = column; r < height && c >= 0; r++, c--) {
		if (board[r * width + c] != symbol)
			break;
		count++;
	}
	for (int r = row, c = column; r >= 0 && c < width; r--, c++) {
		if (board[r * width + c] != symbol)
			break;
		count++;
	}
	if (count > 4)
		return true;

	return false;
}

static int __connect4_ai_score(char *board, int height, int width, int row, int column, char symbol)
{
	int score = 0;
	bool blocked = false;
	int count = 0;

	if (row >= 3) {
		// up
		for (int r = row; r > row - 4; r--) {
			if (board[r * width + column] == symbol)
				count++;
			else if (board[r * width + column] != 0)
				blocked = true;
		}

		if (!blocked)
			score += count*count*count*count*count*count;

		if (column < width - 3) {
			// up and right
			blocked = false;
			count = 0;

			for (int r = row, c = column; r > row - 4; r--, c++) {
				if (board[r * width + c] == symbol)
					count++;
				else if (board[r * width + c] != 0)
					blocked = true;
			}

			if (!blocked)
				score += count*count*count*count*count*count;
		}
	}

	if (column < width - 3) {
		// right
		blocked = false;
		count = 0;

		for (int c = column; c < column + 4; c++) {
			if (board[row * width + c] == symbol)
				count++;
			else if (board[row * width + c] != 0)
				blocked = true;
		}

		if (!blocked)
			score += count*count*count*count*count*count;

		if (row <= height - 4) {
			// down and right
			blocked = false;
			count = 0;

			for (int r = row, c = column; c < column + 4; r++, c++) {
				if (board[r * width + c] == symbol)
					count++;
				else if (board[r * width + c] != 0)
					blocked = true;
			}

			if (!blocked)
				score += count*count*count*count*count*count;
		}
	}

	return score;
}

static int calculate_score(char *board, int height, int width, char symbol)
{
	int score = 0;

	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++)
			score += __connect4_ai_score(board, height, width, r, c, symbol);
	}

//	printf("%c score %d\n", symbol, score);

	return score;
}

static int next_move_score_max(char *board, int height, int width, char myself, char opponent, int depth)
{
//	printf("max call %d\n", depth);

	int best_score = 0x80000000;

	for (int i = 0; i < width; i++) {
		int row = drop(board, height, width, i, myself);
		if (row == -1)
			continue;

		if (win(board, height, width, row, i)) {
			pickup(board, width, i, row);
			return 0x7fffffff - depth;
		}

		int score;
		if (depth >= MAX_DEPTH)
			score = calculate_score(board, height, width, myself) - calculate_score(board, height, width, opponent) * 2;
		else
			score = next_move_score_min(board, height, width, myself, opponent, ++depth);

		pickup(board, width, i, row);

		if (score > best_score)
			best_score = score;
	}

//	printf("max finish %d\n", depth);

	return best_score;
}

static int next_move_score_min(char *board, int height, int width, char myself, char opponent, int depth)
{
//	printf("min call %d\n", depth);

	int best_score = 0x7fffffff;

	for (int i = 0; i < width; i++) {
		int row = drop(board, height, width, i, opponent);
		if (row == -1)
			continue;

		if (win(board, height, width, row, i)) {
			pickup(board, width, i, row);
			return 0x80000000 + depth;
		}

		int score;
		if (depth >= MAX_DEPTH)
			score = calculate_score(board, height, width, myself) - calculate_score(board, height, width, opponent) * 2;
		else
			score = next_move_score_max(board, height, width, myself, opponent, ++depth);

		pickup(board, width, i, row);

		if (score < best_score)
			best_score = score;
	}

//	printf("min finish %d\n", depth);

	return best_score;
}

extern int connect4_ai(char *board, int height, int width, char myself)
{
	int best_score = 0x80000000;
	int best_column = -1;

	char opponent = 'o';
	if (myself == 'o')
		opponent = 'x';

	for (int i = 0; i < width; i++) {
		int row = drop(board, height, width, i, myself);
		if (row == -1)
			continue;

		if (win(board, height, width, row, i)) {
			pickup(board, width, i, row);
//			printf("win %d\n", i);
			return i;
		}

//		int score = calculate_score(board, height, width, myself) - calculate_score(board, height, width, opponent) * 2;

		int score = next_move_score_min(board, height, width, myself, opponent, 1);

//		printf("->\t\t%d score %d\n", i, score);

		pickup(board, width, i, row);

		if (score >= best_score) {
			best_score = score;
			best_column = i;
		}
	}


//	printf("select %d, score %d\n", best_column, best_score);
	return best_column;
}

