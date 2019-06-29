/** Copyright (C) EndeavorBio, Inc. - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Code contained herein is proprietary and confidential.

@file ProgressBar.cc
@brief Draws a progress bar to the screen during execution
@author Alex Sevy (alex@endeavorbio.com)
*/

#include <functional>
#include <iostream>
#include <mutex>


#include "ProgressBar.hh"

using namespace std;

namespace errorx {

ProgressBar::ProgressBar() : 
	total_( 0 ),
	processed_( 0 )
	{}


ProgressBar::ProgressBar( int total ) :
	total_( total ),
	processed_( 0 )
	{}

ProgressBar::ProgressBar( ProgressBar const & other ) :
	total_( other.total_ ),
	processed_( other.processed_ )
	{}

void ProgressBar::increment( unsigned int value, unsigned int total, std::mutex* m ) {
	m->lock();
	processed_ += value;
	total_ = total;
	draw();
	m->unlock();
}

// void ProgressBar::increment( unsigned int value, unsigned int total ) { 
// 	processed_ += value; 
// 	total_ = total;
// 	draw();
// }

void ProgressBar::update( unsigned int value, unsigned int total, std::mutex* m ) {
	m->lock();
	processed_ = value;
	total_ = total;
	draw();
	m->unlock();
}

// void ProgressBar::update( unsigned int value, unsigned int total ) { 
// 	processed_ = value;
// 	total_ = total;
// 	draw();
// }

void ProgressBar::reset() { processed( 0 ); }

void ProgressBar::finish() {
	processed_ = total_;
	draw();
} 

void ProgressBar::draw() {
	float progress = (float)processed_/(float)total_;

	cout << "[";
	int barWidth = 70;
	int pos = barWidth * progress;

	for (int ii = 0; ii < barWidth; ++ii) {
		if (ii < pos) cout << "=";
		else if (ii == pos) cout << ">";
		else cout << " ";
	}

	cout << "] " << int(progress * 100.0) << "%: " << processed_ << "/" << total_ << " records processed \r"; 

	cout.flush();
}

int ProgressBar::total() const { return total_; }
int ProgressBar::processed() const { return processed_; }

void ProgressBar::total( int const & total ) { total_ = total; }
void ProgressBar::processed( int const & processed ) { processed_ = processed; }

void ProgressBar::blank( unsigned int value, unsigned int total, std::mutex* m ) {};
void ProgressBar::blank2() {};

} // namespace errorx


