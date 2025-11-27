#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <string>
#include <algorithm>

void simulateLRU(int numFrames, const std::vector<int>& referenceString) 
{
    // List stores usage order (MRU at front, LRU at back).
    std::list<int> pageList; 
    // MAP 1: Maps page number to its iterator in the pageList for O(1) hit/update.
    std::unordered_map<int, std::list<int>::iterator> pageListIteratorMap;
    
    // Vector represents the physical frames for output purposes.
    std::vector<int> framesDisplay(numFrames, -1); // Initialize frames to -1 (empty)
    // MAP 2: Maps page number to its fixed index in the framesDisplay vector.
    std::unordered_map<int, int> framePositions; 

    int pageFaults = 0;
    int nextInsertIndex = 0; // Tracks the next available index for initial filling

    // Output header
    std::cout << "Page replacement using LRU\n"; 

    // LRU Simulation Loop
    for (int i = 0; i < referenceString.size(); ++i) 
    {
        int currentPage = referenceString[i];

        // Check for Page Hit (The page is present in a frame)
        if (pageListIteratorMap.count(currentPage)) 
        {
            
            // Move the page to the MRU position (front of pageList)
            auto it = pageListIteratorMap[currentPage];
            pageList.splice(pageList.begin(), pageList, it); 
            
        } 
        else 
        {
            // Page Fault
            pageFaults++;
            
            // Replacement or Initial Fill
            if (pageList.size() == numFrames) 
            {
                // Frames are full: Replacement
                
                // Identify the LRU page (at the back of pageList).
                int lruPage = pageList.back();
                
                // Find its physical frame index
                int physicalIndex = framePositions[lruPage];
                
                // Remove LRU page from all logical structures
                pageList.pop_back(); 
                pageListIteratorMap.erase(lruPage);
                framePositions.erase(lruPage);
                
                // Insert the new page into the vacated physical frame
                framesDisplay[physicalIndex] = currentPage;
                
                // The physical index for the new page is the index of the one just replaced
                framePositions[currentPage] = physicalIndex; 
                
            } 
            else 
            {
                // Frames are not full: Initial Fill
                
                // Insert the new page into the next available physical frame slot
                framesDisplay[nextInsertIndex] = currentPage;
                
                // Map the new page to this physical index
                framePositions[currentPage] = nextInsertIndex;
                
                // Advance the index counter
                nextInsertIndex++;
            }
            
            // Insert the new page to MRU in the pageList
            pageList.push_front(currentPage);
            pageListIteratorMap[currentPage] = pageList.begin();
        }

        // Output Formatting for Time Step (uses the fixed-position framesDisplay vector)
        std::cout << "time step " << i << ": ";
        for (int p : framesDisplay) 
        {
            // Only print if the frame is occupied
            if (p != -1) 
            {
                std::cout << p << " ";
            }
        }
        std::cout << "\n";
    }

    // Total Page Faults
    std::cout << "total number of page faults = " << pageFaults;
}

int main() 
{
    std::string keyword;
    int numFrames;
    
    if (!(std::cin >> keyword >> numFrames) || keyword != "Frames") 
    {
        std::cerr << "Error: Invalid input format. Expected 'Frames [number]'\n";
        return 1;
    }

    std::vector<int> referenceString;
    int page;
    while (std::cin >> page) 
    {
        referenceString.push_back(page);
    }

    // Print the reference string for the very first output line
    for (int p : referenceString) 
    {
        std::cout << p << " ";
    }
    std::cout << "\n";

    simulateLRU(numFrames, referenceString);

    return 0;
}