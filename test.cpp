#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <string>
#include <algorithm>

/**
 * @brief Simulates the LRU algorithm, ensuring the output matches the required
 * display format (pages are printed based on their fixed physical frame index).
 * * @param numFrames The maximum number of frames available.
 * @param referenceString The sequence of pages referenced.
 */
void simulateLRU(int numFrames, const std::vector<int>& referenceString) {
    // LOGIC Structure: List stores usage order (MRU at front, LRU at back).
    std::list<int> pageList; 
    // MAP 1: Maps page number to its iterator in the pageList for O(1) hit/update.
    std::unordered_map<int, std::list<int>::iterator> pageListIteratorMap;
    
    // DISPLAY Structure: Vector represents the physical frames for output purposes.
    std::vector<int> framesDisplay(numFrames, -1); // Initialize frames to -1 (empty)
    // MAP 2: Maps page number to its fixed index in the framesDisplay vector.
    std::unordered_map<int, int> framePositions; 

    int pageFaults = 0;
    int nextInsertIndex = 0; // Tracks the next available index for initial filling

    // Output header
    std::cout << "Page replacement using LRU\n"; 

    // 2. LRU Simulation Loop
    for (int i = 0; i < referenceString.size(); ++i) {
        int currentPage = referenceString[i];

        // A. Check for Page Hit (The page is present in a frame)
        if (pageListIteratorMap.count(currentPage)) {
            
            // Move the page to the MRU position (front of pageList)
            auto it = pageListIteratorMap[currentPage];
            pageList.splice(pageList.begin(), pageList, it); 
            
            // Since it's a hit, the framesDisplay vector does not change.
            
        } else {
            // B. Page Fault
            pageFaults++;
            
            // Replacement or Initial Fill
            if (pageList.size() == numFrames) {
                // Frames are full: Replacement
                
                // 1. Identify the LRU page (at the back of pageList).
                int lruPage = pageList.back();
                
                // 2. Find its physical frame index
                int physicalIndex = framePositions[lruPage];
                
                // 3. Remove LRU page from all logical structures
                pageList.pop_back(); 
                pageListIteratorMap.erase(lruPage);
                framePositions.erase(lruPage);
                
                // 4. Insert the new page into the vacated physical frame
                framesDisplay[physicalIndex] = currentPage;
                
                // The physical index for the new page is the index of the one just replaced
                framePositions[currentPage] = physicalIndex; 
                
            } else {
                // Frames are not full: Initial Fill
                
                // 1. Insert the new page into the next available physical frame slot
                framesDisplay[nextInsertIndex] = currentPage;
                
                // 2. Map the new page to this physical index
                framePositions[currentPage] = nextInsertIndex;
                
                // 3. Advance the index counter
                nextInsertIndex++;
            }
            
            // Regardless of replacement/fill, insert the new page to MRU in the pageList
            pageList.push_front(currentPage);
            pageListIteratorMap[currentPage] = pageList.begin();
        }

        // 3. Output Formatting for Time Step (uses the fixed-position framesDisplay vector)
        std::cout << "time step " << i << ": ";
        for (int p : framesDisplay) {
            // Only print if the frame is occupied
            if (p != -1) {
                std::cout << p << " ";
            }
        }
        std::cout << "\n";
    }

    // Final Output: Total Page Faults
    std::cout << "total number of page faults = " << pageFaults;
}

int main() {
    // --- Input Processing ---
    std::string keyword;
    int numFrames;
    
    if (!(std::cin >> keyword >> numFrames) || keyword != "Frames") {
        std::cerr << "Error: Invalid input format. Expected 'Frames [number]'\n";
        return 1;
    }

    std::vector<int> referenceString;
    int page;
    while (std::cin >> page) {
        referenceString.push_back(page);
    }

    // Print the reference string for the very first output line
    for (int p : referenceString) {
        std::cout << p << " ";
    }
    std::cout << "\n";
    
    // --- Simulation ---
    simulateLRU(numFrames, referenceString);

    return 0;
}