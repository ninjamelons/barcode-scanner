namespace BScanner {
    namespace Types {
        class Threshold
        {
        public:
            Threshold(unsigned short blur, unsigned short threshold);
            ~Threshold();
            unsigned short GetBlur();
            unsigned short GetThreshold();
        private:
            unsigned short blur;
            unsigned short threshold;
        };
    }
}