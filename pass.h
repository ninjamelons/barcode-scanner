namespace BScanner {
    namespace Types {
        class Pass
        {
        public:
            Pass(Threshold passFar, Threshold passNear);
            ~Pass();
            unsigned short GetBlur();
            unsigned short GetThreshold();
        private:
            Threshold passFar;
            Threshold passNear;
        };
    }
}