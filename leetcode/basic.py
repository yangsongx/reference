class Solution(object):

    # the first easy question
    def twoSum(self, nums, target):
        """
        :type nums: List[int]
        :type target: int
        :rtype: List[int]
        """
        print(nums)
        print(target)
        a = 0
        b = 0
        found = False
        print("--dump end")
        for i in range(0, len(nums) - 1):
            for j in range(1, len(nums)):
                if nums[i] + nums[j] == target and i != j:
                    print("Wow, it works!")
                    a = i
                    b = j
                    found = True
                    break
            if found == True:
                break

        return [a, b]

    def isPalindrome(self, x):
        """
        :type x: int
        :rtype: bool
        """
        # first , i need convert it to string,
        # directly use int is more difficult than converting string
        data = str(x)
        rev = ''
        for i in range(0, len(data)):
            print(i)
            rev = rev + data[len(data) - 1 - i]

        print('%s V.S. %s' %(data, rev))
        if data == rev:
            return True
        else:
            return False

    

    def start(self):
        self.isPalindrome(121)
        return 0

s = Solution()
s.start()
