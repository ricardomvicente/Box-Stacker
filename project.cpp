#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdio>

struct Box {
    int width;
    int length;
    int height;

    Box(int w, int l, int h) {
        width = std::min(w, l);
        length = std::max(w, l);
        height = h;
    }
};


struct BoxState {
    int max_height;
    int count;
    int next_idx;

    BoxState() {
        max_height = 0;
        count = 1;
        next_idx = -1;
    }
};


struct SegmentTree {
    int num_leaves;
    std::vector<int> tree_height;
    std::vector<int> tree_idx;

    SegmentTree(int n) {
        num_leaves = n;
        tree_height.assign(4 * n, 0);
        tree_idx.assign(4 * n, -1);
    }
};


bool compareBoxes(const Box& a, const Box& b) {
    if (a.width != b.width) return a.width < b.width;
    if (a.length != b.length) return a.length > b.length;
    return a.height > b.height;
}


std::vector<int> calulate_unique_length(std::vector<Box> boxes, int num_boxes) {
    std::vector<int> unique_length;
    unique_length.reserve(num_boxes);

    for (int i = 0; i < num_boxes; i++) {
        unique_length.push_back(boxes[i].length);
    }

    std::sort(unique_length.begin(), unique_length.end());

    unique_length.erase(
        std::unique(unique_length.begin(), unique_length.end()),
        unique_length.end()
    );

    return unique_length;
}


std::pair<int, int> getMaxHeight(int node, int left_node, int right_node,
                                 int target_left, int target_right, SegmentTree& tree) {
    if (right_node < target_left || left_node > target_right) {
        return {0, -1};
    }

    if (left_node >= target_left && right_node <= target_right) {
        return {tree.tree_height[node], tree.tree_idx[node]};
    }

    int mid = left_node + (right_node - left_node) / 2;

    std::pair<int, int> left_result = getMaxHeight(2 * node, left_node, mid, target_left, target_right, tree);
    std::pair<int, int> right_result = getMaxHeight(2 * node + 1, mid + 1, right_node, target_left, target_right, tree);

    if (left_result.first >= right_result.first) {
        return left_result;
    }
    else {
        return right_result;
    }

}


void updateMaxHeight(int node, int left_node, int right_node, int target_pos,
                     int new_height, int box_idx, SegmentTree& tree) {
    if (left_node == right_node) {
        if (new_height > tree.tree_height[node]) {
            tree.tree_height[node] = new_height;
            tree.tree_idx[node] = box_idx;
        }
        return;
    }

    int mid = left_node + (right_node - left_node) / 2;

    if (target_pos <= mid) {
        updateMaxHeight(2 * node, left_node, mid, target_pos, new_height, box_idx, tree);
    }
    else {
        updateMaxHeight(2 * node + 1, mid + 1, right_node, target_pos, new_height, box_idx, tree);
    }

    if (tree.tree_height[2 * node] >= tree.tree_height[2 * node + 1]) {
        tree.tree_height[node] = tree.tree_height[2 * node];
        tree.tree_idx[node] = tree.tree_idx[2 * node];
    }
    else {
        tree.tree_height[node] = tree.tree_height[2 * node + 1];
        tree.tree_idx[node] = tree.tree_idx[2 * node + 1];
    }

}


void print_box(Box box) {
    printf("%d %d %d\n", box.width, box.length, box.height);
}

void print_result(int max_h, int num_blocks, int start_idx,
                  const std::vector<BoxState>& state, const std::vector<Box>& boxes) {
    printf("%d %d\n", max_h, num_blocks);

    int curr = start_idx;

    while (curr != -1) {
        print_box(boxes[curr]);
        curr = state[curr].next_idx;
    }
}


int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    std::vector<Box> boxes;
    boxes.reserve(3 * n);

    for (int i = 0; i < n ; i++) {
        int w, l, h;
        
        if (scanf("%d %d %d", &w, &l, &h) != 3) return 0;

        boxes.push_back(Box(w, l, h));
        boxes.push_back(Box(h, w, l));
        boxes.push_back(Box(l, h, w));
    }

    std::sort(boxes.begin(), boxes.end(), compareBoxes);

    int num_boxes = boxes.size();

    std::vector<int> unique_length = calulate_unique_length(boxes, num_boxes);
    int num_unique_length = unique_length.size();
    SegmentTree tree(num_unique_length);
    std::vector<BoxState> state(num_boxes);


    for (int i = 0; i < num_boxes; i++) {
        int pos = std::lower_bound(unique_length.begin(), unique_length.end(), boxes[i].length) - unique_length.begin();
        int best_h = 0;
        int best_idx = -1;
        
        if (pos > 0) {
            std::pair<int, int> res = getMaxHeight(1, 0, num_unique_length - 1, 0, pos - 1, tree);
            best_h = res.first;
            best_idx = res.second;
        }

        state[i].max_height = boxes[i].height + best_h;


        if (best_idx != -1) {
            state[i].count = 1 + state[best_idx].count;
            state[i].next_idx = best_idx;
        }

        updateMaxHeight(1, 0, num_unique_length - 1, pos, state[i].max_height, i, tree);
    }

    
    int result_height = tree.tree_height[1];
    int result_FirstIdx = tree.tree_idx[1];

    int total_boxes = 0;
    if (result_FirstIdx != -1) {
        total_boxes = state[result_FirstIdx].count;
    }

    print_result(result_height, total_boxes, result_FirstIdx, state, boxes);

    return 0;
}